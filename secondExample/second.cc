/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"
// Default Network Topology
//
//       172.16.1.0
// n0 -------------- n1   n2   n3   n4
//    point-to-point  |    |    |    |
//                    ================
//                      LAN 172.16.2.0

// Using Namespace NS3
using namespace ns3;
// Documentation Only
NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

int 
main (int argc, char *argv[])
{
  // Verbosity {Telling what is running!}
  bool verbose = true;
  // Unsigned int 32 bits
  uint32_t nCsma = 3;

  CommandLine cmd (__FILE__);
  // n2 n3 n4 are cSma Nodes, n1 is P2P node
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
 
  cmd.Parse (argc,argv);
  
  // If Verbose is True log to terminal!
  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }
  
  nCsma = nCsma == 0 ? 1 : nCsma;

  NodeContainer p2pNodes;
  p2pNodes.Create (2);
  
  NodeContainer csmaNodes;
  // Add n1 to to csma Nodes n1 is a csmaNode as well as P2P Node
  csmaNodes.Add (p2pNodes.Get (1));
  csmaNodes.Create (nCsma); // nCsma=2 already declared

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

  // Csma helper calss, DataRate is 100Mbps and Delay is 6560 Nano Seconds
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  // NICs to CSMA Nodes
  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  // Add nodes to network
  InternetStackHelper stack;
  stack.Install (p2pNodes.Get (0)); // Add P2P Node
  stack.Install (csmaNodes); // Add CSMA Nodes (Inc. n1)

  Ipv4AddressHelper address;
  address.SetBase ("172.16.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);

  address.SetBase ("172.16.2.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);

  UdpEchoServerHelper echoServer (1234);
  
  // CSMA Nodes Get(3) is n4 (It is made the server)
  ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  // Setup Client
  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma), 1234);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (3));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (2048));
  // Assign Client Apps to n0
  ApplicationContainer clientApps = echoClient.Install (p2pNodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // For PCAP
  // P2P Channel all nodes have pcap
  pointToPoint.EnablePcapAll ("p2p");
  // CSMA Channel only nodes n2 and n4 are sniffed
  csma.EnablePcap ("csma1", csmaDevices.Get (1), true);
  csma.EnablePcap ("csma3", csmaDevices.Get (3), true);
  
  // Animation
  AnimationInterface anim("second.xml");
  anim.SetConstantPosition(p2pNodes.Get(0),10.0,10.0);
  anim.SetConstantPosition(csmaNodes.Get(0),20.0,20.0);
  anim.SetConstantPosition(csmaNodes.Get(1),30.0,30.0);
  anim.SetConstantPosition(csmaNodes.Get(2),40.0,40.0);
  anim.SetConstantPosition(csmaNodes.Get(3),50.0,50.0);
  
  // ASCII Trace
  AsciiTraceHelper ascii;
  pointToPoint.EnableAsciiAll(ascii.CreateFileStream("p2p.tr"));
  csma.EnableAsciiAll(ascii.CreateFileStream("csma.tr"));
  
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
