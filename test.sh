for i in $(seq 1 100); do
    ./a.out 4 410 200 100 100 5 0 fifo > /dev/null || echo "FAILED run $i"
done
echo "done"