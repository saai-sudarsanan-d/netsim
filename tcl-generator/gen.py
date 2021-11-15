import random

f = open("wsn-py.tcl","w")

nn = 10 #int(input("Enter number of nodes: "))

f.writelines(f"""
# Step 1 : Initialise Variables

set val(chan)	Channel/WirelessChannel		;# Channel Type
set val(prop)	Propagation/TwoRayGround	;# Radio-Propagation Model WAVELAN DSSS
set val(netif) 	Phy/WirelessPhy			;# Network Interface Type
set val(mac) 	Mac/SMAC			;# MAC Type
set val(ifq) 	Queue/DropTail/PriQueue		;# Interface Queue Type
set val(ll) 	LL				;# Link Layer Type
set val(ant) 	Antenna/OmniAntenna		;# Antenna Model
set val(ifqlen) 100				;# Max packet in ifq
set val(nn) 	{nn}				;# Number of Mobile Nodes
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
$ns node-config -adhocRouting $val(rp) \\
		-llType $val(ll) \\
		-macType $val(mac) \\
		-ifqType $val(ifq) \\
		-ifqLen $val(ifqlen) \\
		-antType $val(ant) \\
		-propType $val(prop) \\
		-phyType $val(netif) \\
		-topoInstance $topo \\
		-agentTrace ON \\
		-macTrace ON \\
		-routerTrace ON \\
		-movementTrace ON \\
		-channel $channel1
""")

for i in range(nn):
	j = random.randint(10,490)
	k = random.randint(10,490)
	f.write(f"set n{i+1} [$ns node]\n")
	f.write(f"$ns initial_node_pos $n{i+1} 20\n")
	f.write(f"$n{i+1} random-motion 0\n")
	f.write(f"$n{i+1} set X_ {j-1}\n")
	f.write(f"$n{i+1} set Y_ {k-1}\n")
	f.write(f"$n{i+1} set Z_ 0.0\n")	
	f.write(f"$ns at 0 \"$n{i+1} setdest {j} {k} 100\"")
	f.write("\n")

cons = {
	1:[6,1,5],
	2:[6,1,5],
	3:[7,1,5],
	4:[7,1,5],
	5:[8,1,5],
}

for k,l in cons.items():
	v = l[0]
	f.writelines(f"""
# UDP {k} >--< {v}
set udp{k} [new Agent/UDP]
set null{v} [new Agent/Null]
$ns attach-agent $n{k} $udp{k}
$ns attach-agent $n{v} $null{v}
$ns connect $udp{k} $null{v}
# CBR {k}
set cbr{k} [new Application/Traffic/CBR]
$cbr{k} attach-agent $udp{k}
$ns at {l[1]} "$cbr{k} start"
$ns at {l[2]} "finish"	
	""")

f.writelines("""
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
""")
