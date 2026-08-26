for i in $(seq 1 20); do
  ./codexion 4 800 200 100 100 5 0 edf | grep "has taken" | head -8
  echo "---"
done