IND=0
TMASS=160
HOME='/afs/cern.ch/user/k/ktaeun/private/rootfile/'
DIR='MADWEIGHT160'
NAME='tmass'
while [ $IND -le 9 ]
do
	root -l -q 'analyze_event.cc('\"$TMASS\"', '\"$DIR$IND\"', '\"$NAME\"')'
	((IND++))
done

