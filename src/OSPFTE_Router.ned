simple OSPFTE_Router
{
    parameters:
        @display("i=device/router");

        // OSPF-TE specific parameters
        string ospfAreaId = "0.0.0.0";
        string routerId  = "";
        double helloInterval = 10;
        double deadInterval = 40;

        // Traffic Engineering parameters
         bool teEnabled = true;
        double bandwidthWeight = 1.0;
        double delayWeight = 1.0;
        double loadWeight= 1.0;

        // TED (Traffic Engineering Database) parameters
        double tedUpdateInterval = 30;

    gates:
        input in[];
        output out[];
}
