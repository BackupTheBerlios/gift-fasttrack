#include "fst_fasttrack.h"
#include "fst_node.h"

/*
 * Global dataset:
 * peer1 => [node1, node2, node3...]
 * peer2 => ...
 *
 * Per-session dataset:
 * peer1 => &node1,
 * peer2 => ...
 *
 * where &node1 is the link of this node in the peer's entry in the global
 * dataset, for efficient removal
 */

static List *add_global (Dataset *gpeers, FSTNode *node, FSTNode *peer)
{
	List *l = dataset_lookup (gpeers, &peer, sizeof(peer));

	assert (node->session);

	l = list_prepend (l, node);

	dataset_insert (&gpeers, &peer, sizeof(peer), l, 0);

	fst_node_ref (peer);

	return l;
}

/* node is the one we're connected to, peer is the one we're not */
/* dataset is keyed by peer, and contains a list of nodes */
static void remove_global (Dataset *gpeers, FSTNode *peer, List *nodelink)
{
	FSTNode *node = nodelink->data;
	List *l, *lnew;

	l = dataset_lookup (gpeers, &peer, sizeof(peer));
	
	assert (l);

	lnew = list_remove_link (l, nodelink);

	fst_node_free (peer);

	if (l == lnew)
		return;

	if (lnew)
		dataset_insert (&gpeers, &peer, sizeof(peer), lnew, 0);
	else
		dataset_remove (gpeers, &peer, sizeof(peer));
}

static int remove_peer (ds_data_t *key, ds_data_t *value,
			 void *udata)
{
	remove_global ((Dataset *)udata, *(FSTNode **)key->data,
		       (List *)value->data);

	return DS_REMOVE;
}

void fst_peer_remove (Dataset *gpeers, FSTNode *node, Dataset *peers)
{
	/* should pass node to remove_peer and assert that 
	 * node == nodelink->data */
	dataset_foreach_ex (peers, DS_FOREACH_EX(remove_peer), gpeers);

	dataset_clear (peers);
}

void fst_peer_insert (Dataset *gpeers, FSTNode *node, Dataset **peers, FSTNode *peer)
{
	List *nodelink = dataset_lookup (*peers, &peer, sizeof(peer));
	List *nl;
	fst_node_ref (peer);

	if (nodelink)
		remove_global (gpeers, peer, nodelink);

	nodelink = add_global (gpeers, node, peer);
	assert (nodelink->prev == NULL || nodelink->prev->next == nodelink);
	dataset_insert (peers, &peer, sizeof(peer), nodelink, 0);

	nl = dataset_lookup (*peers, &peer, sizeof (peer));
	assert (nl == nodelink);
	fst_node_free (peer);
}
