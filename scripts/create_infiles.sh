#!/bin/bash

# Prepei na dw8oun akribws 4 arguments
if [ $# != 4 ]; then
	echo "Wrong number of arguments !!!"
	exit 1
fi
###

# 1. Elegxw ta noumera eisodou
# Elegxw an to 2o argument (num_of_files) einai akeraios ari8mos kai megaluteros h isos apo to 0
NUM_OF_FILES=$2
#https://stackoverflow.com/questions/806906/how-do-i-test-if-a-variable-is-a-number-in-bash
re='^[0-9]+$'
if ! [[ $NUM_OF_FILES =~ $re ]]; then
	echo "Argument num_of_files is not a number !!!"
	exit 2
fi
if (( $NUM_OF_FILES < 0 )); then 
	echo "num_of_files must be positive !!!"
	exit 3
fi
###

# Elegxw an to 3o argument (num_of_dirs) einai akeraios ari8mos kai megaluteros h isos apo to 0
NUM_OF_DIRS=$3
#https://stackoverflow.com/questions/806906/how-do-i-test-if-a-variable-is-a-number-in-bash
re='^[0-9]+$'
if ! [[ $NUM_OF_DIRS =~ $re ]]; then
	echo "Argument num_of_dirs is not a number !!!"
	exit 4
fi
if (( $NUM_OF_DIRS < 0 )); then
	echo "num_of_dirs must be positive !!!"
	exit 5
fi
###

# Elegxw an to 4o argument (levels) einai akeraios ari8mos kai megaluteros h isos apo to 0
LEVELS=$4
#https://stackoverflow.com/questions/806906/how-do-i-test-if-a-variable-is-a-number-in-bash
re='^[0-9]+$'
if ! [[ $LEVELS =~ $re ]]; then
	echo "Argument levels is not a number !!!"
	exit 4
fi
if (( $LEVELS < 0 )); then
	echo "levels must be positive !!!"
	exit 5
fi
###


# 2. An to dir_name den uparxei to dhmiourgw
# Elegxw an uparxei to directory (dir_name) , einai sthn metablhth $DIR_NAME
DIR_NAME=$1
#https://stackoverflow.com/questions/638975/how-do-i-tell-if-a-regular-file-does-not-exist-in-bash
if [ ! -d $DIR_NAME ]; then
	#DIR NOT FOUND , opote ton dhmiourgw
	mkdir ${DIR_NAME}
fi
###

# 3. Dhmiourgw ta directory names
DIRECTORY_NAMES=()
COUNTER=0
while [ $COUNTER -lt $NUM_OF_DIRS ]; do
	XARAKTHRES=$RANDOM
	XARAKTHRES=$(($XARAKTHRES%8))
	let XARAKTHRES=XARAKTHRES+1

	#https://gist.github.com/earthgecko/3089509
	NEW_DIR_NAME=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $XARAKTHRES | head -n 1)
	#echo "NEW_DIR_NAME = "$NEW_DIR_NAME

	DIRECTORY_NAMES+=("$NEW_DIR_NAME")

	let COUNTER=COUNTER+1
done

# 4. Ftianxw ta dirs
DIRECTORIES=()
#arxika bazw to DIR_NAME
DIRECTORIES+=("$DIR_NAME")
COUNTER=0
while [ $COUNTER -lt $NUM_OF_DIRS ]; do
	#an to counter einai pollaplasio tou LEVELS tote prepei na arxisw pali apo thn arxh na bazw dirs
	if !(($COUNTER % $LEVELS)); then
		NEW_DIR_NAME=$DIR_NAME"/"${DIRECTORY_NAMES[$COUNTER]}
		#echo $NEW_DIR_NAME
	#alliws pairnw ton teleutaio dir pou eftiaksa kai tou bazw ena epipodo akoma
	else
		NEW_DIR_NAME=${DIRECTORIES[$COUNTER]}"/"${DIRECTORY_NAMES[$COUNTER]}
		#echo $NEW_DIR_NAME
	fi

	#dhmiourgw to dir_name kai to bazw sthn lista me ta directories
	mkdir ${NEW_DIR_NAME}
	DIRECTORIES+=("$NEW_DIR_NAME")

	let COUNTER=COUNTER+1
done

# 5. Ftiaxnw ta arxeia
FILES=()
COUNTER=0
while [ $COUNTER -lt $NUM_OF_FILES ]; do
	let "OFFSET=COUNTER % (NUM_OF_DIRS+1)"

	XARAKTHRES=$RANDOM
	XARAKTHRES=$(($XARAKTHRES%8))
	let XARAKTHRES=XARAKTHRES+1

	#https://gist.github.com/earthgecko/3089509
	NEW_FILE=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $XARAKTHRES | head -n 1)
	#echo "NEW_FILE = "$NEW_FILE

	PATH_FILE=${DIRECTORIES[OFFSET]}"/"${NEW_FILE}
	#echo $PATH_FILE
	FILES+=("$PATH_FILE")

	let COUNTER=COUNTER+1
done

# 6.Gemizw ta arxeia
COUNTER=0
while [ $COUNTER -lt $NUM_OF_FILES ]; do
	#https://stackoverflow.com/questions/2556190/random-number-from-a-range-in-a-bash-script
	#to range ths random einai 0 and 32767

	XARAKTHRES=$RANDOM
	let XARAKTHRES=XARAKTHRES*32768+XARAKTHRES #ousistika ekana XARAKTHRES<<15+XARAKTHRES , me skopo na ftiaksw enan ari8mo 2^30 , egw xreiazomai ws 2^17 , pollaplasiazw me 2^15
	XARAKTHRES=$(($XARAKTHRES%130048)) #mou dinei ari8mous sto (0,127*1024(127kb))
	let XARAKTHRES=XARAKTHRES+1024 #twra exw sto (1024,128*1024)=(1 kb , 128 kb)
	CONTEX=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $XARAKTHRES | head -n 1)
	touch ${FILES[COUNTER]}
	echo ${CONTEX} >> ${FILES[COUNTER]}

	let COUNTER=COUNTER+1
done
