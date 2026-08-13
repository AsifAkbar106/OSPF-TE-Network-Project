simple VideoTrafficGenerator
{
    parameters:
        @display("i=block/app");

        // Video traffic characteristics
        int packetLength = 1500; // Large packets for video
        double sendInterval @unit(s) = exponential(0.033s); // ~30 FPS
        double startTime @unit(s) = uniform(0s, 1s);
        double stopTime @unit(s) = 60s;

        // Video quality parameters
        double bitrate  = 5000000; // High bitrate for video (5Mbps)
         int burstSize = 1; // Reduced burst size to avoid channel conflicts

    gates:
        input in[];
        output out[];
}
