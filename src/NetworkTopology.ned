network NetworkTopology
{
    parameters:
        @display("bgb=800,600");

    submodules:
        // Hosts for traffic generation
        host1: SimpleHost {
            @display("p=158,66");
        }
        host2: SimpleHost {
            @display("p=700,100");
        }
        host3: SimpleHost {
            @display("p=100,500");
        }
        host4: SimpleHost {
            @display("p=700,500");
        }

        // OSPF-TE Routers
        ospfRouter1: OSPFTE_Router {
            @display("p=200,200");
        }
        ospfRouter2: OSPFTE_Router {
            @display("p=400,200");
        }
        ospfRouter3: OSPFTE_Router {
            @display("p=200,400");
        }
        ospfRouter4: OSPFTE_Router {
            @display("p=400,400");
        }
        ospfRouter5: OSPFTE_Router {
            @display("p=300,300");
        }

        // RIP Routers
        ripRouter1: RIP_Router {
            @display("p=500,200");
        }
        ripRouter2: RIP_Router {
            @display("p=600,200");
        }
        ripRouter3: RIP_Router {
            @display("p=500,400");
        }
        ripRouter4: RIP_Router {
            @display("p=600,400");
        }

        // Traffic Generators
        videoTraffic: VideoTrafficGenerator {
            @display("p=40,76");
        }
        voiceTraffic: VoiceTrafficGenerator {
            @display("p=40,167");
        }
        dataTraffic: DataTrafficGenerator {
            @display("p=50,250");
        }

    connections:
        // Host connections to OSPF-TE network
        host1.out++ --> {  datarate = 100Mbps; delay = 1ms; } --> ospfRouter1.in++;
        ospfRouter1.out++ --> {  datarate = 100Mbps; delay = 1ms; } --> host1.in++;

        host2.out++ --> {  datarate = 100Mbps; delay = 1ms; } --> ospfRouter4.in++;
        ospfRouter4.out++ --> {  datarate = 100Mbps; delay = 1ms; } --> host2.in++;

        host3.out++ --> {  datarate = 100Mbps; delay = 1ms; } --> ospfRouter3.in++;
        ospfRouter3.out++ --> {  datarate = 100Mbps; delay = 1ms; } --> host3.in++;

        host4.out++ --> {  datarate = 100Mbps; delay = 1ms; } --> ospfRouter4.in++;
        ospfRouter4.out++ --> {  datarate = 100Mbps; delay = 1ms; } --> host4.in++;

        // OSPF-TE Router connections (high bandwidth, low latency)
        ospfRouter1.out++ --> {  datarate = 1Gbps; delay = 2ms; } --> ospfRouter2.in++;
        ospfRouter2.out++ --> {  datarate = 1Gbps; delay = 2ms;  } --> ospfRouter1.in++;

        ospfRouter1.out++ --> {  datarate = 1Gbps; delay = 2ms;  } --> ospfRouter3.in++;
        ospfRouter3.out++ --> {  datarate = 1Gbps; delay = 2ms; } --> ospfRouter1.in++;

        ospfRouter2.out++ --> {  datarate = 1Gbps; delay = 2ms;  } --> ospfRouter5.in++;
        ospfRouter5.out++ --> {  datarate = 1Gbps; delay = 2ms;  } --> ospfRouter2.in++;

        ospfRouter3.out++ --> {  datarate = 1Gbps; delay = 2ms;  } --> ospfRouter5.in++;
        ospfRouter5.out++ --> {  datarate = 1Gbps; delay = 2ms;  } --> ospfRouter3.in++;

        ospfRouter4.out++ --> {  datarate = 1Gbps; delay = 2ms;  } --> ospfRouter5.in++;
        ospfRouter5.out++ --> {  datarate = 1Gbps; delay = 2ms;  } --> ospfRouter4.in++;

        ospfRouter2.out++ --> {  datarate = 1Gbps; delay = 2ms;  } --> ospfRouter4.in++;
        ospfRouter4.out++ --> {  datarate = 1Gbps; delay = 2ms;  } --> ospfRouter2.in++;

        // RIP Router connections (simple topology)
        ripRouter1.out++ --> {  datarate = 100Mbps; delay = 5ms;  } --> ripRouter2.in++;
        ripRouter2.out++ --> {  datarate = 100Mbps; delay = 5ms; } --> ripRouter1.in++;

        ripRouter1.out++ --> {  datarate = 100Mbps; delay = 5ms;  } --> ripRouter3.in++;
        ripRouter3.out++ --> {  datarate = 100Mbps; delay = 5ms;  } --> ripRouter1.in++;

        ripRouter2.out++ --> {  datarate = 100Mbps; delay = 5ms;  } --> ripRouter4.in++;
        ripRouter4.out++ --> {  datarate = 100Mbps; delay = 5ms;  } --> ripRouter2.in++;

        ripRouter3.out++ --> {  datarate = 100Mbps; delay = 5ms; } --> ripRouter4.in++;
        ripRouter4.out++ --> {  datarate = 100Mbps; delay = 5ms;  } --> ripRouter3.in++;

        // Connection between OSPF-TE and RIP networks
        ospfRouter2.out++ --> {  datarate = 100Mbps; delay = 10ms;  } --> ripRouter1.in++;
		ripRouter1.out++ --> {  datarate = 100Mbps; delay = 10ms;  } --> ospfRouter2.in++;
        // Traffic generator connections
        videoTraffic.out++ --> {  datarate = 100Mbps; delay = 1ms; } --> host1.in++;
        host1.out++ --> {  datarate = 100Mbps; delay = 1ms; } --> videoTraffic.in++;

        voiceTraffic.out++ --> {  datarate = 100Mbps; delay = 1ms; } --> host2.in++;
        host2.out++ --> {  datarate = 100Mbps; delay = 1ms; } --> voiceTraffic.in++;

        dataTraffic.out++ --> {  datarate = 100Mbps; delay = 1ms; } --> host3.in++;
        host3.out++ --> {  datarate = 100Mbps; delay = 1ms; } --> dataTraffic.in++;


}
