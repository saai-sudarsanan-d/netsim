#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("THIRD-SCRIPT-EXAMPLE");

int main(int argc, char *argv[]){
    bool verbose=true;
    uint32_t nCsma = 3;
    uint32_t nWifi = 3;
    bool tracing = true;

    CommandLine  cmd;
    cmd.AddValue ("nCsma","Number of extra CSMA nodes/devices: ",nCsma);
    cmd.AddValue ("nWifi","Number of Wifi STA Devices:",nWifi);
    cmd.AddValue ("verbose","Tell echo applications to log if true", verbose);

    cmd.Parse(argc,argv);
    if (nWifi > 18) {
            std::cout << "nWifi should be less than 18 or less otherwise grid layout exceeds bounding box" << std::endl;
            return 1;
        }
    if (verbose){
            LogComponentEnable ("UdpEchoClientApplication",LOG_LEVEL_INFO);
            LogComponentEnable ("UdpEchoServerApplication",LOG_LEVEL_INFO);
    }

    NodeContainer p2pNodes;
    p2pNodes.Create(2);

    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
    p2p.SetChannelAttribute("Delay",StringValue("2ms"));

    NetDeviceContainer p2pDevices;
    p2pDevices = p2p.Install(p2pNodes);

    NodeContainer csmaNodes;
    csmaNodes.Add (p2pNodes.Get(1));
    csmaNodes.Create (nCsma);

    CsmaHelper csma;
    csma.SetChannelAttribute ("DataRate",StringValue ("100Mbps"));
    csma.SetChannelAttribute ("Delay",TimeValue(NanoSeconds(6560)));

    NetDeviceContainer csmaDevices;
    csmaDevices = csma.Install(csmaNodes);

    // Wireless Nodes
    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(nWifi);
    // Wireless Access Point
    // YANS - Yet Another Network Simulator
    NodeContainer apNode = p2pNodes.Get(0);
    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phy;
    phy.SetChannel(channel.Create ());

    WifiHelper wifi;
    wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

    WifiMacHelper mac;
    Ssid ssid = Ssid ("ns3-3-ssid");
    mac.SetType ("ns3::StaWifiMac","Ssid",SsidValue(ssid),
                "ActiveProbing",BooleanValue (false));

    NetDeviceContainer staDevices;
    staDevices = wifi.Install (phy,mac,wifiStaNodes);

    mac.SetType ("ns3::ApWifiMac","Ssid",SsidValue (ssid));
    NetDeviceContainer apDevices;
    apDevices = wifi.Install(phy,mac,apNode);

    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                    "MinX",DoubleValue (0.0),
                                    "MinY",DoubleValue(0.0),
                                    "DeltaX",DoubleValue(5.0),
                                    "DeltaY",DoubleValue(10.0),
                                    "GridWidth",UintegerValue(3),
                                    "LayoutType",StringValue("RowFirst"));
    mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                                "Bounds",RectangleValue(Rectangle(-50,50,-50,50)));
    mobility.Install (wifiStaNodes);

    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(apNode);

    InternetStackHelper stack;
    stack.Install(csmaNodes);
    stack.Install(apNode);
    stack.Install(wifiStaNodes);

    Ipv4AddressHelper address;

    address.SetBase ("172.11.1.0","255.255.255.0");
    Ipv4InterfaceContainer csmaInterfaces;
    csmaInterfaces = address.Assign(csmaDevices);

    address.SetBase ("172.11.2.0","255.255.255.0");
    Ipv4InterfaceContainer p2pInterface;
    p2pInterface = address.Assign(p2pDevices);

    address.SetBase ("172.11.3.0","255.255.255.0");
    Ipv4InterfaceContainer staInterface;
    Ipv4InterfaceContainer apInterface;
    staInterface = address.Assign(staDevices);
    apInterface = address.Assign(apDevices);

    UdpEchoServerHelper echoServer(3456);

    ApplicationContainer serverApps = echoServer.Install(csmaNodes.Get(nCsma));
    serverApps.Start(Seconds (1.0));
    serverApps.Stop(Seconds (10.0));

    UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress(nCsma),3456);
    echoClient.SetAttribute("MaxPackets",UintegerValue(3));
    echoClient.SetAttribute("Interval",TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize",UintegerValue(2048));

    ApplicationContainer clientApps = echoClient.Install(wifiStaNodes.Get(nWifi - 1));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    Simulator::Stop(Seconds(10.0));
    if (tracing == true){
        p2p.EnablePcapAll("p2p");
        phy.EnablePcap("phy",apDevices.Get(0));
        csma.EnablePcap("csma",csmaDevices.Get(0),true);
    }
    AsciiTraceHelper ascii;
    p2p.EnableAsciiAll(ascii.CreateFileStream("p2p.tr"));
    csma.EnableAsciiAll(ascii.CreateFileStream("csma.tr"));
    phy.EnableAsciiAll(ascii.CreateFileStream("wireless.tr"));
    Simulator::Run();
    Simulator::Destroy();
}
