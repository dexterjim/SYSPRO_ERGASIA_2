#!/bin/bash

echo "HELLO WORLD"

IDS=()
NUMBER_OF_IDS=0
MAX_ID=0
MIN_ID=0
BYTES_SEND=0
BYTES_RECEIVED=0
FILES_SEND=0
FILES_RECEIVED=0
NUMBER_OF_CLIENTS_LEFT=0

#https://linuxhint.com/read_file_line_by_line_bash/
#diabazw grammh grammh to cat
while read line; do
	echo "-----------------"
	echo $line;

	WHICH_WORD=0
	TYPE=0

	for word in $line
	do
		echo $word

		if [ $WHICH_WORD -eq 0 ]; then
			WHICH_WORD=1
			if [ "$word" = "id" ]; then
				TYPE=1
			elif [ "$word" = "send" ]; then
				TYPE=2
			elif [ "$word" = "receive" ]; then
				TYPE=3
			elif [ "$word" = "client" ]; then
				let NUMBER_OF_CLIENTS_LEFT=NUMBER_OF_CLIENTS_LEFT+1
				break
			fi
			echo "TYPE ="$TYPE
		elif [ $WHICH_WORD -eq 1 ]; then
			WHICH_WORD=2
			if [ $TYPE -eq 1 ]; then
				IDS+=("$word")
				let NUMBER_OF_IDS=NUMBER_OF_IDS+1
				if [ $MAX_ID -eq 0 ]; then
					let MAX_ID=word
				elif [ $MAX_ID -lt $word ]; then
					let MAX_ID=word
				fi

				if [ $MIN_ID -eq 0 ]; then
					let MIN_ID=word
				elif [ $MIN_ID -gt $word ]; then
					let MIN_ID=word
				fi
			elif [ $TYPE -eq 2 ]; then
				#https://stackoverflow.com/questions/806906/how-do-i-test-if-a-variable-is-a-number-in-bash
				re='^[0-9]+$'
				if [[ $word =~ $re ]]; then #an einai ari8mos
					echo "Aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
					let BYTES_SEND=BYTES_SEND+word
				fi
			elif [ $TYPE -eq 3 ]; then
				#https://stackoverflow.com/questions/806906/how-do-i-test-if-a-variable-is-a-number-in-bash
				re='^[0-9]+$'
				if [[ $word =~ $re ]]; then #an einai ari8mos
					let BYTES_RECEIVED=BYTES_RECEIVED+word
				fi
			fi
		elif [ $WHICH_WORD -eq 2 ]; then
			if [ $TYPE -eq 2 ]; then
				let BYTES_SEND=BYTES_SEND+word
				let FILES_SEND=FILES_SEND+1
			elif [ $TYPE -eq 3 ]; then
				let BYTES_RECEIVED=BYTES_RECEIVED+word
				let FILES_RECEIVED=FILES_RECEIVED+1
			fi
		fi
		
	done
done

echo "Number of ids : "$NUMBER_OF_IDS
echo -ne "ids :"
COUNTER=0
while [ $COUNTER -lt $NUMBER_OF_IDS ]; do
	echo -ne " "${IDS[COUNTER]}
	let COUNTER=COUNTER+1
done
echo
echo "Max id : "$MAX_ID
echo "Min id : "$MIN_ID
echo "Bytes send : "$BYTES_SEND
echo "Bytes received : "$BYTES_RECEIVED
echo "Files send : "$FILES_SEND
echo "Files received : "$FILES_RECEIVED
echo "Number of clients left the system : "$NUMBER_OF_CLIENTS_LEFT
