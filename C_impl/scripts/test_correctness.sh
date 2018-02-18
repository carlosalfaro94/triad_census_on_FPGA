#!/bin/bash

source init.sh

make rand
exe/rand_graph_gen 7 20 graphs/graph_nodes_7_edges_20.txt
make parallel

#echo "testing correctess of single_BF"
#exe/main_parallel -g "graphs/graph_nodes_7_edges_20.txt" -b "../../binaries_old/tc_single_BF.aocx" -f "single_BF" -r
#echo "testing correctess of single_BF_ord"
#exe/main_parallel -g "graphs/graph_nodes_7_edges_20.txt" -b "binaries/tc_single_BF_ord.aocx" -f "single_BF_ord" -o -r
echo "testing correctess of single_BM"
exe/main_parallel -g "graphs/graph_nodes_7_edges_20.txt" -b "binaries/tc_single_BM.aocx" -f "single_BM" -r
echo "testing correctess of single_BM_ord"
exe/main_parallel -g "graphs/graph_nodes_7_edges_20.txt" -b "binaries/tc_single_BM_ord.aocx" -f "single_BM_ord" -o -r
#echo "testing correctess of NDRange_BF"
#exe/main_parallel -g "graphs/graph_nodes_7_edges_20.txt" -b "binaries/tc_NDRange_BF.aocx" -f "NDRange_BF" -r
#echo "testing correctess of NDRange_BF_ord"
#exe/main_parallel -g "graphs/graph_nodes_7_edges_20.txt" -b "binaries/tc_NDRange_BF_ord.aocx" -f "NDRange_BF_ord" -o -r
#echo "testing correctess of NDRange_BM"
#exe/main_parallel -g "graphs/graph_nodes_7_edges_20.txt" -b "binaries/tc_NDRange_BM.aocx" -f "NDRange_BM" -r
echo "testing correctess of NDRange_BM_ord"
exe/main_parallel -g "graphs/graph_nodes_7_edges_20.txt" -b "binaries/tc_NDRange_BM_ord.aocx" -f "NDRange_BM_ord" -o -r

make clean_graphs
make clean
