// Aqui está o código da aplicação que visa implementar uma topologia de barra em qu o primeiro nó envia um aquivo que vai passando de nó em nó até chegar no nó 4, e ao chegar no nó 4, volta até o segundo nó e o ciclo se reinicia. 
// vale comentar que fiz esse código pensando no NS3 3.35 como base, então talvez o compilador de outra versão do ns3 nao não funcione.

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("TopologiaBarra");

uint16_t porta = 8080;
Ptr<Socket> sendSockets[5];
Ipv4InterfaceContainer interfaces;
Ptr<UniformRandomVariable> randGen;
AnimationInterface *animPtr = nullptr;

void EnviarParaVizinho(uint32_t senderIndex, uint32_t receiverIndex, uint32_t valor);
void ReceberPacote(Ptr<Socket> socket);
bool AcceptCallback(Ptr<Socket>, const Address &) { return true; }
void NovaConexao(Ptr<Socket> socket, const Address &) {
    socket->SetRecvCallback(MakeCallback(&ReceberPacote));
}
//Não consegui deixar os 30 segundos exatos em tempo de terminal, mas em visualizadores como no netanim é possivel ver que toda simulação realmente dura 30 segundos.
void IniciarEnvio(uint32_t senderIndex, uint32_t receiverIndex) {
    if (Simulator::Now().GetSeconds() >= 30.0) return;

    uint32_t valor = randGen->GetInteger(1, 100);
    NS_LOG_UNCOND("Nó " << senderIndex << " gerou valor " << valor);
    EnviarParaVizinho(senderIndex, receiverIndex, valor);
}

void EnviarParaVizinho(uint32_t senderIndex, uint32_t receiverIndex, uint32_t valor) {
    uint8_t buffer[4];
    memcpy(buffer, &valor, 4);
    Ptr<Packet> packet = Create<Packet>(buffer, 4);

    Ptr<Socket> socket = Socket::CreateSocket(sendSockets[senderIndex]->GetNode(), TcpSocketFactory::GetTypeId());
    Ipv4Address destIp = interfaces.GetAddress(receiverIndex);
    InetSocketAddress remote = InetSocketAddress(destIp, porta);
    socket->Connect(remote);
    socket->Send(packet);
}

void ReceberPacote(Ptr<Socket> socket) {
    Address from;
    Ptr<Packet> packet = socket->RecvFrom(from);
    uint8_t buffer[4];
    packet->CopyData(buffer, 4);
    uint32_t valor;
    memcpy(&valor, buffer, 4);

    Ptr<Node> thisNode = socket->GetNode();
    uint32_t nodeId = thisNode->GetId();
    NS_LOG_UNCOND("Nó " << nodeId << " recebeu valor: " << valor );

    InetSocketAddress address = InetSocketAddress::ConvertFrom(from);
    Ipv4Address origem = address.GetIpv4();

    if (nodeId == 4) {
        uint32_t novoValor = randGen->GetInteger(1, 100);
        NS_LOG_UNCOND("Nó 4 gerou novo valor: " << novoValor);
        Simulator::Schedule(Seconds(1.0), &EnviarParaVizinho, 4, 3, novoValor);
    } else if (nodeId == 1 && origem == interfaces.GetAddress(2)) {
        uint32_t novoValor = randGen->GetInteger(1, 100);
        NS_LOG_UNCOND("Nó 1 gerou novo valor: " << novoValor);
        Simulator::Schedule(Seconds(1.0), &EnviarParaVizinho, 1, 2, novoValor);
    } else {
        uint32_t proximo;
        if (nodeId == 0 || origem == interfaces.GetAddress(nodeId - 1)) {
            proximo = nodeId + 1;
        } else {
            proximo = nodeId - 1;
        }

        Simulator::Schedule(Seconds(1.0), &EnviarParaVizinho, nodeId, proximo, valor);
    }
}

int main() {
    NodeContainer nodes;
    nodes.Create(5);

    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(nodes);

    for (uint32_t i = 0; i < 5; ++i) {
        nodes.Get(i)->GetObject<MobilityModel>()->SetPosition(Vector(50.0 * i, 0.0, 0.0));
    }

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phy = YansWifiPhyHelper();
    phy.SetChannel(channel.Create());

    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211b);
    wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode", StringValue("DsssRate1Mbps"));

    WifiMacHelper mac;
    mac.SetType("ns3::AdhocWifiMac");

    NetDeviceContainer devices = wifi.Install(phy, mac, nodes);

    InternetStackHelper stack;
    stack.Install(nodes);

    Ipv4AddressHelper address;
    address.SetBase("10.0.0.0", "255.0.0.0");
    interfaces = address.Assign(devices);

    // Criação dos sockets, a parte que mais deu trabalho no código junto da implementação da lógica do looping.
    for (uint32_t i = 0; i < 5; ++i) {
        sendSockets[i] = Socket::CreateSocket(nodes.Get(i), TcpSocketFactory::GetTypeId());

        Ptr<Socket> recvSocket = Socket::CreateSocket(nodes.Get(i), TcpSocketFactory::GetTypeId());
        InetSocketAddress local = InetSocketAddress(Ipv4Address::GetAny(), porta);
        recvSocket->Bind(local);
        recvSocket->Listen();
        recvSocket->SetAcceptCallback(MakeCallback(&AcceptCallback), MakeCallback(&NovaConexao));
    }

    randGen = CreateObject<UniformRandomVariable>();
    randGen->SetAttribute("Min", DoubleValue(1));
    randGen->SetAttribute("Max", DoubleValue(100));
    AnimationInterface anim("topologia-barra.xml"); //Vai gerar um arquivo xml caso queira testar no simulador netanim depois, mas já vou avisando que a visualização no netanim está meio cru.

    for (uint32_t i = 0; i < 5; ++i) {
        anim.SetConstantPosition(nodes.Get(i), 50.0 * i, 20.0);
        anim.UpdateNodeDescription(i, "Nó " + std::to_string(i));
        anim.UpdateNodeColor(i, 0, 255, 0);
    }

    //Não consegui encaixar as imagens para utilizar no netanim, então deixei essa parte caso vc queira mudar e deixar a visualização mais bonitinha.
    uint32_t pcIcon = anim.AddResource("pc.png");
    uint32_t serverIcon = anim.AddResource("server.png");


    anim.UpdateNodeImage(0, serverIcon);
    anim.UpdateNodeImage(1, pcIcon);
    anim.UpdateNodeImage(2, pcIcon);
    anim.UpdateNodeImage(3, pcIcon);
    anim.UpdateNodeImage(4, pcIcon);


    Simulator::Schedule(Seconds(1.0), &IniciarEnvio, 0, 1);

    Simulator::Stop(Seconds(30.0));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
