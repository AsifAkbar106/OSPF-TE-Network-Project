simple SimpleRouter
{
    parameters:
        @display("i=device/router");

        // Basic router parameters
        string routerId = "";

    gates:
        input in[];
        input io[];
}
