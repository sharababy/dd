
echo "Starting Toooling Script"

echo "installing python3.7"


#sudo apt-get install python3.7

pyver=$(python3.7 -V)

echo $pyver

if "$pyver"=="Python 3.7.1";
then
	echo Yes
fi
