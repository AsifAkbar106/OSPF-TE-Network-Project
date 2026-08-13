simple RIP_Router
{
    parameters:
        @display("i=device/router");

        // RIP specific parameters
        double updateInterval= 30;
        double invalidTimer = 180;
        double flushTimer= 240;
        int maxHopCount = 15;

    gates:
        input in[];
        output out[];
}
