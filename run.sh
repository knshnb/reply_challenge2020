g++ -O3 -std=c++17 main.cpp
for c in {0..5}; do
echo $c | ./a.out &
done
