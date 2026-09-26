/*
 * Copyright (c) 2011-2018 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */

#include "ns3/buildings-helper.h"
#include "ns3/config-store.h"
#include "ns3/core-module.h"
#include "ns3/lte-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/buildings-helper.h"
#include "ns3/config-store.h"
#include "ns3/netanim-module.h"
// #include "ns3/gtk-config-store.h"

using namespace ns3;

int main(int argc, char* argv[])
{
    Time simTime = MilliSeconds(1050);
    bool useCa = false;

    CommandLine cmd(__FILE__);
    cmd.AddValue("simTime", "Total duration of the simulation", simTime);
    cmd.AddValue("useCa", "Whether to use carrier aggregation.", useCa);
    cmd.Parse(argc, argv);

    ConfigStore inputConfig;
    inputConfig.ConfigureDefaults();

    // Parse again so you can override default values from the command line
    cmd.Parse(argc, argv);

    if (useCa)
    {
        Config::SetDefault("ns3::LteHelper::UseCa", BooleanValue(useCa));
        Config::SetDefault("ns3::LteHelper::NumberOfComponentCarriers", UintegerValue(2));
        Config::SetDefault("ns3::LteHelper::EnbComponentCarrierManager",
                           StringValue("ns3::RrComponentCarrierManager"));
    }

    Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();
    Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper>();
    lteHelper->SetEpcHelper(epcHelper);

    // Create Nodes: eNodeB and UE
    NodeContainer enbNodes;
    NodeContainer ueNodes;
    enbNodes.Create(1);
    ueNodes.Create(3);

    // Install Mobility Model
    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(enbNodes);
    BuildingsHelper::Install(enbNodes);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(ueNodes);
    BuildingsHelper::Install(ueNodes);

    // Create Devices and install them in the Nodes (eNB and UE)
    NetDeviceContainer enbDevs;
    NetDeviceContainer ueDevs;

    enbDevs = lteHelper->InstallEnbDevice(enbNodes);
    ueDevs = lteHelper->InstallUeDevice(ueNodes);

    InternetStackHelper internet;
    internet.Install(ueNodes);

    // Attach a UE to a eNB
    lteHelper->Attach(ueDevs, enbDevs.Get(0));

    Ipv4InterfaceContainer ueIpIface;
    ueIpIface = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueDevs));

    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    AnimationInterface anim("mophong_bai10_setting.xml");

    // (option) set position to node in netanim
    anim.SetConstantPosition(enbNodes.Get(0), 0.0, 0.0);
    anim.SetConstantPosition(ueNodes.Get(0), 20.0, 0.0);
    anim.SetConstantPosition(ueNodes.Get(0), 40.0, 0.0);
    anim.SetConstantPosition(ueNodes.Get(0), 60.0, 0.0);

    Simulator::Stop(simTime);

    Simulator::Run();

    monitor->SerializeToXmlFile("mophong_bai10_flowmon.xml", true, true);

    Simulator::Destroy();
    return 0;
}

