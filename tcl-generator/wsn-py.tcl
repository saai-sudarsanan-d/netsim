
# Step 1 : Initialise Variables

set val(chan)	Channel/WirelessChannel		;# Channel Type
set val(prop)	Propagation/TwoRayGround	;# Radio-Propagation Model WAVELAN DSSS
set val(netif) 	Phy/WirelessPhy			;# Network Interface Type
set val(mac) 	Mac/SMAC			;# MAC Type
set val(ifq) 	Queue/DropTail/PriQueue		;# Interface Queue Type
set val(ll) 	LL				;# Link Layer Type
set val(ant) 	Antenna/OmniAntenna		;# Antenna Model
set val(ifqlen) 100				;# Max packet in ifq
set val(nn) 	10				;# Number of Mobile Nodes
set val(rp) 	AODV				;# Routing Protocol
set val(x) 	500				;# In Metres
set val(y) 	500				;# In Metres

# Step 2 : Create a Simulator Object

set ns [new Simulator]

# Step 3 : Create Tracing and Animation File

set tracefile [open out.tr w]
$ns trace-all $tracefile

set namfile [open out.nam w]
$ns namtrace-all-wireless $namfile $val(x) $val(y)

# Step 4 : Topography

set topo [new Topography]
$topo load_flatgrid $val(x) $val(y)

# Step 5 : GOD - General Operations Director
# To tell the Wireless nodes what to do, when?
create-god $val(nn)

# Step 7 : Create Channel (Communication Path)

set channel1 [new $val(chan)]

# Step 6 : Create Nodes
# Configuration
$ns node-config -adhocRouting $val(rp) \
		-llType $val(ll) \
		-macType $val(mac) \
		-ifqType $val(ifq) \
		-ifqLen $val(ifqlen) \
		-antType $val(ant) \
		-propType $val(prop) \
		-phyType $val(netif) \
		-topoInstance $topo \
		-agentTrace ON \
		-macTrace ON \
		-routerTrace ON \
		-movementTrace ON \
		-channel $channel1
set n1 [$ns node]
$ns initial_node_pos $n1 20
$n1 random-motion 0
$n1 set X_ 412
$n1 set Y_ 335
$n1 set Z_ 0.0
$ns at 0 "$n1 setdest 413 336 100"
set n2 [$ns node]
$ns initial_node_pos $n2 20
$n2 random-motion 0
$n2 set X_ 170
$n2 set Y_ 434
$n2 set Z_ 0.0
$ns at 0 "$n2 setdest 171 435 100"
set n3 [$ns node]
$ns initial_node_pos $n3 20
$n3 random-motion 0
$n3 set X_ 141
$n3 set Y_ 462
$n3 set Z_ 0.0
$ns at 0 "$n3 setdest 142 463 100"
set n4 [$ns node]
$ns initial_node_pos $n4 20
$n4 random-motion 0
$n4 set X_ 82
$n4 set Y_ 438
$n4 set Z_ 0.0
$ns at 0 "$n4 setdest 83 439 100"
set n5 [$ns node]
$ns initial_node_pos $n5 20
$n5 random-motion 0
$n5 set X_ 287
$n5 set Y_ 67
$n5 set Z_ 0.0
$ns at 0 "$n5 setdest 288 68 100"
set n6 [$ns node]
$ns initial_node_pos $n6 20
$n6 random-motion 0
$n6 set X_ 401
$n6 set Y_ 459
$n6 set Z_ 0.0
$ns at 0 "$n6 setdest 402 460 100"
set n7 [$ns node]
$ns initial_node_pos $n7 20
$n7 random-motion 0
$n7 set X_ 51
$n7 set Y_ 76
$n7 set Z_ 0.0
$ns at 0 "$n7 setdest 52 77 100"
set n8 [$ns node]
$ns initial_node_pos $n8 20
$n8 random-motion 0
$n8 set X_ 172
$n8 set Y_ 370
$n8 set Z_ 0.0
$ns at 0 "$n8 setdest 173 371 100"
set n9 [$ns node]
$ns initial_node_pos $n9 20
$n9 random-motion 0
$n9 set X_ 335
$n9 set Y_ 151
$n9 set Z_ 0.0
$ns at 0 "$n9 setdest 336 152 100"
set n10 [$ns node]
$ns initial_node_pos $n10 20
$n10 random-motion 0
$n10 set X_ 286
$n10 set Y_ 101
$n10 set Z_ 0.0
$ns at 0 "$n10 setdest 287 102 100"

# UDP 1 >--< 6
set udp1 [new Agent/UDP]
set null6 [new Agent/Null]
$ns attach-agent $n1 $udp1
$ns attach-agent $n6 $null6
$ns connect $udp1 $null6
# CBR 1
set cbr1 [new Application/Traffic/CBR]
$cbr1 attach-agent $udp1
$ns at 1 "$cbr1 start"
$ns at 5 "finish"	
	
# UDP 2 >--< 6
set udp2 [new Agent/UDP]
set null6 [new Agent/Null]
$ns attach-agent $n2 $udp2
$ns attach-agent $n6 $null6
$ns connect $udp2 $null6
# CBR 2
set cbr2 [new Application/Traffic/CBR]
$cbr2 attach-agent $udp2
$ns at 1 "$cbr2 start"
$ns at 5 "finish"	
	
# UDP 3 >--< 7
set udp3 [new Agent/UDP]
set null7 [new Agent/Null]
$ns attach-agent $n3 $udp3
$ns attach-agent $n7 $null7
$ns connect $udp3 $null7
# CBR 3
set cbr3 [new Application/Traffic/CBR]
$cbr3 attach-agent $udp3
$ns at 1 "$cbr3 start"
$ns at 5 "finish"	
	
# UDP 4 >--< 7
set udp4 [new Agent/UDP]
set null7 [new Agent/Null]
$ns attach-agent $n4 $udp4
$ns attach-agent $n7 $null7
$ns connect $udp4 $null7
# CBR 4
set cbr4 [new Application/Traffic/CBR]
$cbr4 attach-agent $udp4
$ns at 1 "$cbr4 start"
$ns at 5 "finish"	
	
# UDP 5 >--< 8
set udp5 [new Agent/UDP]
set null8 [new Agent/Null]
$ns attach-agent $n5 $udp5
$ns attach-agent $n8 $null8
$ns connect $udp5 $null8
# CBR 5
set cbr5 [new Application/Traffic/CBR]
$cbr5 attach-agent $udp5
$ns at 1 "$cbr5 start"
$ns at 5 "finish"	
	
proc finish {} {
	global ns tracefile namfile
	$ns flush-trace
	close $tracefile
	close $namfile
	exit 0
}

# RUN SIMULATION
puts "STARTING SIMULATION..."
$ns run
