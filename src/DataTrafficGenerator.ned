simple DataTrafficGenerator
{
    parameters:
        @display("i=block/app");

        // Data traffic characteristics
        int packetLength = 512; // Medium packets for data
        double sendInterval @unit(s) = exponential(0.1s); // Variable intervals
        double startTime @unit(s) = uniform(0s, 1s);
        double stopTime @unit(s) = 60s;

        // Data transfer parameters
        double bitrate = 1000000; // Medium bitrate for data (1Mbps)
        double burstInterval @unit(s) = exponential(5s); // Bursty with longer intervals

    gates:
        input in[];
        output out[];
}