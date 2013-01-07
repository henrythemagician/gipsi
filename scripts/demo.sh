cd ../topodebug
java -jar StreamingViz.jar &

cd
ssh -X -t telpc32 'workspace/P2Pgpac/gipsi/scripts/startup-source.sh' | tee workspace/P2Pgpac/gipsi/log/telpc32.txt &
sleep 3

cd
ssh -X -t telpc21 'workspace/P2Pgpac/gipsi/scripts/startup-peer.sh' | tee workspace/P2Pgpac/gipsi/log/telpc21.txt &
sleep 3

cd
ssh -X -t telpc23 'workspace/P2Pgpac/gipsi/scripts/startup-peer.sh' | tee workspace/P2Pgpac/gipsi/log/telpc23.txt &
sleep 3

cd
ssh -X -t telpc33 'workspace/P2Pgpac/gipsi/scripts/startup-peer.sh' | tee workspace/P2Pgpac/gipsi/log/telpc33.txt &
sleep 3

cd
ssh -X -t telpc34 'workspace/P2Pgpac/gipsi/scripts/startup-peer.sh' | tee workspace/P2Pgpac/gipsi/log/telpc34.txt &
sleep 3

cd
ssh -X -t telpc35 'workspace/P2Pgpac/gipsi/scripts/startup-peer.sh' | tee workspace/P2Pgpac/gipsi/log/telpc35.txt &
sleep 3

cd
ssh -X -t telpc36 'workspace/P2Pgpac/gipsi/scripts/startup-peer.sh' | tee workspace/P2Pgpac/gipsi/log/telpc36.txt &
sleep 3

cd
ssh -X -t telpc37 'workspace/P2Pgpac/gipsi/scripts/startup-peer.sh' | tee workspace/P2Pgpac/gipsi/log/telpc37.txt &
sleep 3


cd
ssh -X -t telpc37 'Desktop/stream.sh'  &
sleep 3


echo 'Input Any Command to Exit Demo'

read inputline
