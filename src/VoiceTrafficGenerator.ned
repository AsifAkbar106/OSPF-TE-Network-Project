simple VoiceTrafficGenerator
{
    parameters:
        @display("i=block/app");

        // Voice traffic characteristics
        int packetLength = 160; // Small packets for voice
        double sendInterval @unit(s) = 0.02s; // 20ms intervals (50 packets/sec)
        double startTime @unit(s) = uniform(0s, 1s);
        double stopTime @unit(s) = 60s;

        // Voice quality parameters
        double bitrate= 64000; // Low bitrate for voice (64kbps)
        double jitterTolerance= 0.05; // Low jitter tolerance (50ms)

    gates:
        input in[];
        output out[];
}
