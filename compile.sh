
NAME="Hexapod_leg"
LIBS=`pkg-config --cflags opencv4`

cd $PWD/scripts/

echo "Cleaning bin..."
rm bin/*

echo "Compiling..."

LINK=""

for file in src/*;
do
    LINK=$LINK"../"$file" "
done

cd ./bin

g++ -c -I ../headers/ $LIBS $LINK

echo "Linking..."

LINK=""

for file in *;
do
    LINK=$LINK$file" "
done

g++ $LINK -o $NAME -pthread `pkg-config --libs opencv4`
echo "Done"