#! /usr/bin/ksh
cd $HOME
. ./.profile
set -e
date_id=`date +%y%m%d%H%M`
TRACE_FILE="compil_unix_$date_id.out"
TRACE_FILE_RES=compil_unix.out
echo "==============================================" >> $TRACE/$TRACE_FILE
echo "       Date  : [`date`]     "  >> $TRACE/$TRACE_FILE
echo "==============================================" >> $TRACE/$TRACE_FILE


if [[ -d "$HOME/src/build" ]]; then
	echo "Deleting $HOME/src/build" >> $TRACE/$TRACE_FILE
	rm -rf $HOME/src/build 1>> $TRACE/$TRACE_FILE 2>>$TRACE/$TRACE_FILE || 
	{
		echo "Error deleting $HOME/src/build" >> $TRACE/$TRACE_FILE;
		cp -f $TRACE/$TRACE_FILE $TRACE/$TRACE_FILE_RES;
		exit 1 ;}
fi

if [[ -d "$HOME/src-spec/build" ]]; then
	echo "Deleting $HOME/src-spec/build" >> $TRACE/$TRACE_FILE
	rm -rf $HOME/src-spec/build 1>> $TRACE/$TRACE_FILE 2>>$TRACE/$TRACE_FILE ||
	{
		echo "Error deleting $HOME/src-spec/build" >> $TRACE/$TRACE_FILE;
		cp -f $TRACE/$TRACE_FILE $TRACE/$TRACE_FILE_RES;
		exit 1; }
fi

if [[ -d "$HOME/src-spec" ]]; then
	cd $HOME/src-spec 1>> $TRACE/$TRACE_FILE 2>>$TRACE/$TRACE_FILE
	echo "Creating $HOME/src-spec/build" >> $TRACE/$TRACE_FILE
	mkdir build 1>> $TRACE/$TRACE_FILE 2>>$TRACE/$TRACE_FILE
	chmod 777 build 1>> $TRACE/$TRACE_FILE 2>>$TRACE/$TRACE_FILE ||
	{
		echo "Error creating $HOME/src-spec/build" >> $TRACE/$TRACE_FILE;
		cp -f $TRACE/$TRACE_FILE $TRACE/$TRACE_FILE_RES;
		exit 1; }
fi

cd $HOME/src
echo "Creating $HOME/src/build" >> $TRACE/$TRACE_FILE
mkdir build 1>> $TRACE/$TRACE_FILE 2>>$TRACE/$TRACE_FILE
chmod 777 build 1>> $TRACE/$TRACE_FILE 2>>$TRACE/$TRACE_FILE ||
	{
	echo "Error creating $HOME/src/build" >> $TRACE/$TRACE_FILE;
	cp -f $TRACE/$TRACE_FILE $TRACE/$TRACE_FILE_RES;
	exit 1;}

if [[ -d "CMakeLists.txt" ]]; then
	export r=$?
	echo "CMakeLists.txt Doesn't exists" >> $TRACE/$TRACE_FILE
	cp -f $TRACE/$TRACE_FILE $TRACE/$TRACE_FILE_RES
	exit $r
fi

if [[ -d "InstallConfig.txt" ]]; then
	export r=$?
	echo "InstallConfig.txt Doesn't exists" >> $TRACE/$TRACE_FILE
	cp -f $TRACE/$TRACE_FILE $TRACE/$TRACE_FILE_RES
	exit $r
fi

echo "Configuring CMake" >> $TRACE/$TRACE_FILE
cd build 1>> $TRACE/$TRACE_FILE 2>>$TRACE/$TRACE_FILE
cmake .. 1>> $TRACE/$TRACE_FILE 2>>$TRACE/$TRACE_FILE ||
    {
		echo "Error Executing CMake" >> $TRACE/$TRACE_FILE;
		cp -f $TRACE/$TRACE_FILE $TRACE/$TRACE_FILE_RES;
		exit 1 ;}

echo "Starting compilation..." >> $TRACE/$TRACE_FILE
make 1>> $TRACE/$TRACE_FILE 2>>$TRACE/$TRACE_FILE ||
	{
		echo "Error Executing make" >> $TRACE/$TRACE_FILE;
		cp -f $TRACE/$TRACE_FILE $TRACE/$TRACE_FILE_RES;
		exit 1;}


echo "Installing..." >> $TRACE/$TRACE_FILE
make install 1>> $TRACE/$TRACE_FILE 2>>$TRACE/$TRACE_FILE ||
	{
		echo "Error Executing make install" >> $TRACE/$TRACE_FILE;
		cp -f $TRACE/$TRACE_FILE $TRACE/$TRACE_FILE_RES;
		exit 1;}

echo "Cleaning..." >> $TRACE/$TRACE_FILE
make clean 1>> $TRACE/$TRACE_FILE 2>>$TRACE/$TRACE_FILE ||
	{
		echo "Error Executing make clean" >> $TRACE/$TRACE_FILE;
		cp -f $TRACE/$TRACE_FILE $TRACE/$TRACE_FILE_RES;
		exit 1;}

echo "===== Ended successfully: [`date`] =========" >> $TRACE/$TRACE_FILE
cp -f $TRACE/$TRACE_FILE $TRACE/$TRACE_FILE_RES


echo "===== Ended successfully: [`date`] ========="