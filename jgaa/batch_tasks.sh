out_dir=$1
graph_list=${out_dir}/list.txt
false_rejection_results=${out_dir}/false_rejection_results.txt
performance_results=${out_dir}/performance_results.txt
false_rejection_plot=${out_dir}/false_rejection_plot.eps
performance_plot=${out_dir}/performance_plot.eps

mesh_sizes=( 10 20 30 40 50 60 70 80 90 100 200 300 400 500 600 700 800 900 1000 )
prop_removed=0.7


echo "============================================="
echo "              Starting Tasks.                "
echo "  Work area: [${out_dir}]"
echo "============================================="
echo ""


echo "============================================="
echo "     Generating Planar Biconnected Graphs    "
echo "============================================="
for v1 in "${mesh_sizes[@]}"
do
    for v2 in "${mesh_sizes[@]}"
    do
        echo "generating [$v1]x[$v2]"
        ../bin/random_planar_graph_generator $v1 $v2 $prop_removed $out_dir
        if [[ $? != 0 ]]
        then
            exit $?
        fi
    done
done

find ${out_dir} -name 'planar_graph_*.txt' > ${graph_list}

echo ""
echo "============================================="
echo "     Experimenting False Rejection           "
echo "============================================="

rm -f ${false_rejection_results}

while IFS='' read -r line || [[ -n "$line" ]]; do
    echo "Testing [$line]"
    ../bin/jgaa_planarity_experiment $line >> ${false_rejection_results} 2>&1
    break
done < $graph_list

echo ""
echo "============================================="
echo "               Measuring Speed               "
echo "============================================="

rm -f ${performance_results}

while IFS='' read -r line || [[ -n "$line" ]]; do
    echo "Testing [$line]"
    ../bin/jgaa_planarity_profiler $line >> ${performance_results} 2>&1

done < $graph_list

rm -f $graph_list


echo ""
echo "============================================="
echo "                   Plotting                  "
echo "============================================="

python plot_false_detection.py ${false_rejection_results} ${false_rejection_plot}

python plot_performance.py ${performance_results} ${performance_plot}

echo ""
echo "False rejection plot is at: ${false_rejection_plot}"
echo "Profiler plot is at: ${performance_plot}"
echo "============================================="
echo "                     Done!                   "
echo "============================================="
