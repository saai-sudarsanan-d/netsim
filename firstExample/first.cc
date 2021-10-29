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
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
// Code for Network Animation
#include "ns3/netanim-module.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//
 // Namespace is set to ns3
using namespace ns3;
// For Documentation Purposes
NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  // Parse Cmd Args
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  // Time resolution is Nano Seconds (NS)
  Time::SetResolution (Time::NS);
  // UDP Component Documentation (Only)
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  
  // Creating 2 Nodes        
  NodeContainer nodes;
  nodes.Create (2);

  // Five entities of NS3 Nodes
  // Node
  // Application
  // Net Device
  // Channel
  // Topology Helpers
  PointToPointHelper pointToPoint; // PointToPoint --> className
  // Wired Network, so DataRate set to 5Mbps and Delay 2ms
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  
  // (Link Layer)(Each NIC has its own MAC)
  // NIC Card creation
  NetDeviceContainer devices;
  // Attach NIC Cards to the Nodes
  devices = pointToPoint.Install (nodes);

  // (Network Layer)(Allocate IPs using InternetStackHelper)
  InternetStackHelper stack;
  // Connect the 2 nodes to the Network
  stack.Install (nodes);
  
  // We use IPv4
  Ipv4AddressHelper address;
  // Set Subnet Mask (10.1.1.0 is the first address and 10.1.1.255 is broadcast address)
  address.SetBase ("10.1.1.0", "255.255.255.0");
  
  // Allocate the interfaces to the devices
  Ipv4InterfaceContainer interfaces = address.Assign (devices);
  
  // Total of 65536 Ports in a Networking Device (1-1024 is reserved)
  // Setup echo Server to listen on port 3689
  UdpEchoServerHelper echoServer (3689);
  
  // echo Server is installed on nodes.Get(1) [Second Node]
  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  // Start Server at 1 seconds and Stop at 10 seconds
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));
  
  // Setup echo Client to send packets to port 3689 
  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 3689);
  // Maximum Number of packet = 1
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  // Interval = 1 seconds
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  // Packet Size = 1024 Bytes
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  // echo Client is installed on nodes.Get(0) [First Node]
  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  // Client Starts at 2 seconds (after server which is at 1 seconds)
  clientApps.Start (Seconds (2.0));
  // Client Stops at 10 seconds
  clientApps.Stop (Seconds (10.0));
  
  // For Animation
  AnimationInterface anim("first.xml");
  anim.SetConstantPosition(nodes.Get(0),10.0,10.0);
  anim.SetConstantPosition(nodes.Get(1),20.0,20.0);
  
  //  Ascii Format Tracing
  AsciiTraceHelper ascii;
  pointToPoint.EnableAsciiAll(ascii.CreateFileStream("first.tr"));
  
  // Run the simulation!
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
