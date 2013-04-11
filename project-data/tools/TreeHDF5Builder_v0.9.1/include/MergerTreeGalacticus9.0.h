void CosmologyGroup(hid_t file_out_hdf5, void *params);
void UnitsGroup(hid_t file_out_hdf5, void *params);
void HaloTreesGroupAttribute(hid_t group_id, void *params);
void HaloTreesGroupDataNew(hid_t file_out_hdf5, MergerTree **halo, int tree_levels, int nb_nodes);
void HaloTreesGroupNew(hid_t file_out_hdf5, MergerTree **halo, int tree_levels, int nb_nodes, void *params);
void TreeIndexGroupNew(hid_t file_out_hdf5, int nb_trees, int nb_nodes, void *params);
