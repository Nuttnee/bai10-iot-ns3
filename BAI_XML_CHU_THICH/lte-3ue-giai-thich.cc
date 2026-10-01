/*
 * SPDX-License-Identifier: GPL-2.0-only
 * Based on the Lesson 10 LTE example (CTTC, Manuel Requena).
 * Ban chu thich: giu bai toan gia nhap mang, KHONG them traffic ung dung.
 * Build khong tao XML; chay chuong trinh moi ghi XML.
 */
#include "ns3/buildings-helper.h"
#include "ns3/core-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/internet-module.h"
#include "ns3/lte-module.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"
#include "ns3/network-module.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

using namespace ns3;

/** Find the actual MME application, without assuming a fixed node ID. */
static Ptr<Node>
FindMme()
{
    for (auto it = NodeList::Begin(); it != NodeList::End(); ++it)
    {
        for (uint32_t j = 0; j < (*it)->GetNApplications(); ++j)
        {
            if (DynamicCast<EpcMmeApplication>((*it)->GetApplication(j)))
            {
                return *it;
            }
        }
    }
    return nullptr;
}

/** Label and color a real node; no decorative nodes or artificial links. */
static void
Label(AnimationInterface& anim, Ptr<Node> node, const std::string& name,
      uint8_t r, uint8_t g, uint8_t b, double size)
{
    anim.UpdateNodeDescription(node, name + " [" + std::to_string(node->GetId()) + "]");
    anim.UpdateNodeColor(node, r, g, b);
    anim.UpdateNodeSize(node, size, size);
}

int
main(int argc, char* argv[])
{
    // Ba tham so cua de: so thiet bi, khoang cach toi tram, thoi gian mo phong.
    uint32_t nUe = 3;
    double dist = 20.0;
    double simTime = 1.05;
    std::string prefix = "mophong_3ue_chuthich";
    CommandLine cmd(__FILE__);
    cmd.AddValue("nUe", "So thiet bi UE (1..30)", nUe);
    cmd.AddValue("dist", "Khoang cach MOI UE toi eNodeB, don vi met", dist);
    cmd.AddValue("simTime", "Thoi gian mo phong, don vi giay (vi du 1.05)", simTime);
    cmd.AddValue("prefix", "Ten dau cua cac file ket qua, khong kem duong dan", prefix);
    cmd.Parse(argc, argv);
    NS_ABORT_MSG_IF(nUe < 1 || nUe > 30, "nUe phai trong khoang 1..30");
    NS_ABORT_MSG_IF(!std::isfinite(dist) || dist <= 0, "dist phai la so duong huu han");
    NS_ABORT_MSG_IF(!std::isfinite(simTime) || simTime <= 0, "simTime phai la so duong huu han");
    NS_ABORT_MSG_IF(prefix.empty() || prefix.find_first_not_of(
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-") != std::string::npos,
        "prefix chi gom chu, so, gach ngang hoac gach duoi");

    auto lte = CreateObject<LteHelper>();
    auto epc = CreateObject<PointToPointEpcHelper>();
    lte->SetEpcHelper(epc);
    // Ghi ro cac mac dinh cua bai goc de nguoi hoc co the doc va doi.
    lte->SetEnbDeviceAttribute("DlBandwidth", UintegerValue(25));
    lte->SetEnbDeviceAttribute("UlBandwidth", UintegerValue(25));
    lte->SetEnbDeviceAttribute("DlEarfcn", UintegerValue(100));
    lte->SetEnbDeviceAttribute("UlEarfcn", UintegerValue(18100));
    lte->SetSchedulerType("ns3::PfFfMacScheduler");

    NodeContainer enbs, ues;
    enbs.Create(1);
    ues.Create(nUe);
    auto enb = enbs.Get(0);
    auto sgw = epc->GetSgwNode();
    auto pgw = epc->GetPgwNode();
    auto mme = FindMme();
    NS_ABORT_MSG_IF(!mme, "Khong tim thay MME");

    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(enbs);
    mobility.Install(ues);
    BuildingsHelper::Install(enbs);
    BuildingsHelper::Install(ues);
    // UE giu nguyen toa do bai goc: tren duong tron ban kinh dist quanh (0,0).
    // Chi toa do cac node EPC duoc sap lai; chung dung lien ket CO DAY.
    AnimationInterface::SetConstantPosition(enb, 0, 0);
    for (uint32_t i = 0; i < nUe; ++i)
    {
        const double angle = 2.0 * std::acos(-1.0) * i / nUe;
        AnimationInterface::SetConstantPosition(ues.Get(i),
            dist * std::cos(angle), dist * std::sin(angle));
    }
    const double spacing = std::max(30.0, dist * 1.5);
    AnimationInterface::SetConstantPosition(sgw, 0, dist + spacing);
    AnimationInterface::SetConstantPosition(pgw, 0, dist + spacing * 2.0);
    AnimationInterface::SetConstantPosition(mme, spacing * 2.5, dist + spacing);

    auto enbDevs = lte->InstallEnbDevice(enbs);
    auto ueDevs = lte->InstallUeDevice(ues);
    InternetStackHelper internet;
    internet.Install(ues);
    lte->Attach(ueDevs, enbDevs.Get(0));
    auto ueIps = epc->AssignUeIpv4Address(ueDevs);

    FlowMonitorHelper fm;
    auto monitor = fm.InstallAll();
    AnimationInterface anim(prefix + "_NETANIM.xml");
    anim.EnablePacketMetadata(true);
    const double nodeSize = spacing * 0.07;
    Label(anim, enb, "eNB: Trạm gốc", 35, 110, 220, nodeSize);
    Label(anim, sgw, "S-GW: Chuyển tiếp", 235, 145, 20, nodeSize);
    Label(anim, pgw, "P-GW: Cổng mạng IP", 15, 150, 170, nodeSize);
    Label(anim, mme, "MME: Quản lý kết nối", 155, 75, 185, nodeSize);
    std::map<uint32_t, std::string> roles;
    roles[enb->GetId()] = "eNodeB";
    roles[sgw->GetId()] = "S-GW";
    roles[pgw->GetId()] = "P-GW";
    roles[mme->GetId()] = "MME";
    for (uint32_t i = 0; i < nUe; ++i)
    {
        const auto role = "UE" + std::to_string(i + 1);
        roles[ues.Get(i)->GetId()] = role;
        Label(anim, ues.Get(i), role + ": Thiết bị LTE", 20, 170, 95, nodeSize);
    }
    anim.UpdateLinkDescription(enb, sgw, "S1-U: đường dữ liệu");
    anim.UpdateLinkDescription(sgw, pgw, "S5: nối hai gateway");
    anim.UpdateLinkDescription(sgw, mme, "S11: báo hiệu");
    // Khong ve day gia UE-eNodeB. S1-MME trong helper la trao doi SAP noi bo,
    // khong phai mot duong IP duoc FlowMonitor ghi nhu S11.

    std::ofstream nodes(prefix + "_nodes.csv");
    nodes << "nodeId,role,x_m,y_m,ip\n";
    for (auto it = NodeList::Begin(); it != NodeList::End(); ++it)
    {
        const auto node = *it;
        const auto pos = node->GetObject<MobilityModel>()->GetPosition();
        const auto ip = node->GetObject<Ipv4>();
        for (uint32_t j = 1; ip && j < ip->GetNInterfaces(); ++j)
        {
            for (uint32_t k = 0; k < ip->GetNAddresses(j); ++k)
            {
                nodes << node->GetId() << ',' << roles.at(node->GetId()) << ','
                      << pos.x << ',' << pos.y << ',' << ip->GetAddress(j, k).GetLocal() << '\n';
            }
        }
    }
    nodes.close();
    auto enbLte = DynamicCast<LteEnbNetDevice>(enbDevs.Get(0));
    std::ofstream settings(prefix + "_settings.txt");
    settings << "nUe=" << nUe << "\ndist_m=" << dist << "\nsimTime_s=" << simTime
             << "\ncellId=" << enbLte->GetCellId()
             << "\nDlBandwidth_RB=" << unsigned(enbLte->GetDlBandwidth())
             << "\nUlBandwidth_RB=" << unsigned(enbLte->GetUlBandwidth())
             << "\nDlEarfcn=" << enbLte->GetDlEarfcn()
             << "\nUlEarfcn=" << enbLte->GetUlEarfcn()
             << "\nscheduler=" << lte->GetSchedulerType()
             << "\napplicationTraffic=false\n";
    settings.close();

    Simulator::Stop(Seconds(simTime));
    Simulator::Run();
    monitor->CheckForLostPackets();
    monitor->SerializeToXmlFile(prefix + "_FLOWMON.xml", true, true);
    auto classifier = DynamicCast<Ipv4FlowClassifier>(fm.GetClassifier());
    std::ofstream csv(prefix + "_flows.csv");
    csv << "flowId,sourceAddress,destinationAddress,sourcePort,destinationPort,protocol,"
           "txPackets,rxPackets,txBytes,rxBytes,lostPackets,delaySum_ns,minDelay_ns,"
           "maxDelay_ns,meanDelay_ns,lastRx_s\n";
    csv << std::setprecision(12);
    uint32_t totalTx = 0, totalRx = 0;
    uint64_t totalBytes = 0;
    for (const auto& [id, s] : monitor->GetFlowStats())
    {
        const auto t = classifier->FindFlow(id);
        csv << id << ',' << t.sourceAddress << ',' << t.destinationAddress << ','
            << t.sourcePort << ',' << t.destinationPort << ',' << unsigned(t.protocol) << ','
            << s.txPackets << ',' << s.rxPackets << ',' << s.txBytes << ',' << s.rxBytes
            << ',' << s.lostPackets << ',' << s.delaySum.GetNanoSeconds() << ',';
        if (s.rxPackets)
        {
            csv << s.minDelay.GetNanoSeconds() << ',' << s.maxDelay.GetNanoSeconds() << ','
                << double(s.delaySum.GetNanoSeconds()) / s.rxPackets << ',' << s.timeLastRxPacket.GetSeconds();
        }
        else
        {
            csv << "NA,NA,NA,NA";
        }
        csv << '\n';
        totalTx += s.txPackets;
        totalRx += s.rxPackets;
        totalBytes += s.rxBytes;
    }
    csv.close();
    Simulator::Destroy();
    std::cout << "\nBAN CHU THICH: " << nUe << " UE, " << dist << " m, " << simTime << " s\n"
              << "MO BANG NETANIM: " << prefix << "_NETANIM.xml\n"
              << "THONG KE, KHONG MO TRONG ANIMATOR: " << prefix << "_FLOWMON.xml\n"
              << "FlowMonitor: tx=" << totalTx << ", rx=" << totalRx << ", rxBytes=" << totalBytes << '\n'
              << "Day la LTE/EPC gia nhap mang; chua co may chu hoac traffic ung dung.\n"
              << "Mui ten IP chu yeu la bao hieu GTP-C, khong phai so do cam bien.\n";
}
