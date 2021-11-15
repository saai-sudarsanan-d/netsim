BEGIN {
        recvdSize = 0
        startTime = 400
        stopTime = 0
        sendLine = 0;
        recvLine = 0;
        fowardLine = 0; 		
        seqno = -1;
	count = 0;
}

  $0 ~/^s.* AGT/ {
        sendLine ++ ;
}
 
$0 ~/^r.* AGT/ {
        recvLine ++ ;
}
 
$0 ~/^f.* RTR/ {
        fowardLine ++ ;
}
  {
             event = $1
             time = $2
             node_id = $3
             pkt_size = $8
             level = $4
	    if($4 == "AGT" && $1 == "s" && seqno < $6) {

          seqno = $6;

    } 
#	else if(($4 == "AGT") && ($1 == "r")) {

#            receivedPackets++;

#    } else if ($1 == "D" && $7 == "tcp" && $8 > 512){

#            droppedPackets++;            

#  } 

    #end-to-end delay

    if($4 == "AGT" && $1 == "s") {

          start_time[$6] = $2;

    } else if(($7 == "cbr") && ($1 == "r")) {

        end_time[$6] = $2;

    } else if($1 == "D" && $7 == "cbr") {

          end_time[$6] = -1;

    }    

  # Store start time
  if ((level == "AGT" || level == "IFQ") && (event == "s") && pkt_size >= 512) {
    if (time < startTime) {
             startTime = time
             }
       }
   
  # Update total received packets' size and store packets arrival time
  if ((level == "AGT" || level == "IFQ") && (event == "r") && pkt_size >= 512) {
       if (time > stopTime) {
             stopTime = time
             }
       # Rip off the header
       #hdr_size = pkt_size % 512
       #pkt_size -= hdr_size
       # Store received packet's size
       recvdSize += pkt_size
       }
  }
    

  END {
	printf("Average Throughput[kbps] = %.2f\t\t StartTime=%.2f\tStopTime=%.2f\n",(recvdSize/(stopTime-startTime))*(8/1000),startTime,stopTime)
	printf("\n")
	printf "s:%d r:%d, r/s Ratio:%.4f, f:%d \n", sendLine, recvLine, (recvLine/sendLine),fowardLine;
	for(i=0; i<=seqno; i++) {

          if(end_time[i] > 0) {

              delay[i] = end_time[i] - start_time[i];

                  count++;

        }

            else

            {

                  delay[i] = -1;

            }

    }

    for(i=0; i<=seqno; i++) {

          if(delay[i] > 0) {

              n_to_n_delay = n_to_n_delay + delay[i];

        }         

    }

   n_to_n_delay = n_to_n_delay/count;

 

    print "\n";

#    print "GeneratedPackets            = " seqno+1;

#    print "ReceivedPackets             = " receivedPackets;

#    print "Packet Delivery Ratio      = " receivedPackets/(seqno+1)*100
#"%";

#    print "Total Dropped Packets = " droppedPackets;

    print "Average End-to-End Delay    = " n_to_n_delay * 1000 " ms";

    print "\n";
	       	
  }
