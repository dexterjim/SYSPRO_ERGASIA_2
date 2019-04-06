#!/bin/bash

IDS=() #lista me ta ids
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
	WHICH_WORD=0
	TYPE=0

	for word in $line #ka8e grammh exei 2-3 lekseis
	do
		if [ $WHICH_WORD -eq 0 ]; then # h prwth leksh exei 4 epiloges pou fainontai parakatw
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
		elif [ $WHICH_WORD -eq 1 ]; then #analoga me thn prwth leksh pou exoume 
			WHICH_WORD=2
			if [ $TYPE -eq 1 ]; then # an htan "id" tote exei akoma 1 leksh pou pou einai to id tou client
				IDS+=("$word") # opote to bazw sthn lista me ta ids , kai ananewnw opou xreiazetai ta upoloipa pou aforoun ta id
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
			elif [ $TYPE -eq 2 ]; then # gia send
				#https://stackoverflow.com/questions/806906/how-do-i-test-if-a-variable-is-a-number-in-bash
				re='^[0-9]+$'
				if [[ $word =~ $re ]]; then #elegxw an einai ari8mos , an nai kseroume oti metafer8hke directory , opote apla auksanoume ta bytes pou steilame
					let BYTES_SEND=BYTES_SEND+word
				fi
			elif [ $TYPE -eq 3 ]; then # gia receive
				#https://stackoverflow.com/questions/806906/how-do-i-test-if-a-variable-is-a-number-in-bash
				re='^[0-9]+$'
				if [[ $word =~ $re ]]; then #omoiws gia receiver
					let BYTES_RECEIVED=BYTES_RECEIVED+word
				fi
			fi
		elif [ $WHICH_WORD -eq 2 ]; then # 3 lekseis exoume mono an metafer8hke arxeio
			if [ $TYPE -eq 2 ]; then # gia send , auksanoume ta bytes poy stal8hkan kai ton ari8mo twn arxeiwn pou stal8hkan
				let BYTES_SEND=BYTES_SEND+word
				let FILES_SEND=FILES_SEND+1
			elif [ $TYPE -eq 3 ]; then # omoiws gia receive
				let BYTES_RECEIVED=BYTES_RECEIVED+word
				let FILES_RECEIVED=FILES_RECEIVED+1
			fi
		fi
		
	done
done

echo "Number of ids : "$NUMBER_OF_IDS
echo -ne "List of ids :"
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
