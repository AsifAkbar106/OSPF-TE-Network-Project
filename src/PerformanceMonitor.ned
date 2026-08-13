simple PerformanceMonitor
{
    parameters:
        @display("i=block/control");

        // Performance monitoring parameters
        double measurementInterval  = 1;
        double convergenceThreshold = 0.1;

        // Output parameters
        string outputFile = "performance_results.vec";
        string statisticsFile= "performance_stats.sca";

    gates:
        inout in[];
        inout out[];
}
