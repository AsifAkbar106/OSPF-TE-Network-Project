# OSPF-TE vs RIP Network Simulation Project

## Project Description

This project implements OSPF with Traffic Engineering (OSPF-TE) to optimize routing based on bandwidth, delay, and load, unlike RIP, which uses hop count. OSPF-TE adjusts routes dynamically in real-time, prioritizing low-latency, high-bandwidth paths. It integrates a Traffic Engineering Database (TED) and Constraint-Based Routing (CBR). The simulation compares OSPF-TE with RIP using traffic types like video, voice, and data, adjusting network parameters. Performance metrics such as convergence time, throughput, latency, and packet loss evaluate both protocols.

## Project Structure

```
2007106_Network_Project/
├── src/
│   ├── NetworkTopology.ned          # Main network topology
│   ├── OSPFTE_Router.ned/.cc/.h     # OSPF-TE router implementation
│   ├── RIP_Router.ned/.cc/.h        # RIP router implementation
│   ├── OSPFTE_Messages.msg          # OSPF-TE message definitions
│   ├── RIP_Messages.msg             # RIP message definitions
│   ├── VideoTrafficGenerator.ned    # Video traffic generator
│   ├── VoiceTrafficGenerator.ned    # Voice traffic generator
│   ├── DataTrafficGenerator.ned     # Data traffic generator
│   ├── PerformanceMonitor.ned/.cc/.h # Performance monitoring
│   └── package.ned                  # Package definition
├── simulations/
│   ├── omnetpp.ini                  # Simulation configurations
│   ├── NetworkTopology.ned          # Simulation network topology
│   └── package.ned                  # Simulation package
├── Makefile                         # Build configuration
└── README.md                        # This file
```

## Key Features

### OSPF-TE Implementation
- **Traffic Engineering Database (TED)**: Maintains link state information including bandwidth, delay, and load
- **Constraint-Based Routing (CBR)**: Calculates optimal paths based on multiple constraints
- **Dynamic Route Adjustment**: Real-time route optimization based on network conditions
- **Multi-metric Routing**: Considers bandwidth, delay, and load in routing decisions

### RIP Implementation
- **Distance Vector Algorithm**: Traditional hop-count based routing
- **Periodic Updates**: Regular routing table updates
- **Simple Metric**: Uses hop count as the primary routing metric

### Traffic Types
- **Video Traffic**: High bandwidth, large packets (1500B), ~30 FPS
- **Voice Traffic**: Low bandwidth, small packets (160B), 20ms intervals
- **Data Traffic**: Medium bandwidth, variable packet sizes (512B)

### Performance Metrics
- **Convergence Time**: Time for routing protocol to stabilize after changes
- **Throughput**: Network data transfer rate
- **Latency**: End-to-end packet delay
- **Packet Loss**: Percentage of lost packets

## Simulation Scenarios

1. **OSPF_TE_Scenario**: Standard OSPF-TE operation with TE enabled
2. **RIP_Scenario**: Standard RIP operation with hop-count routing
3. **HighLoad_Scenario**: High traffic load conditions
4. **LinkFailure_Scenario**: Network link failure and recovery
5. **BandwidthVariation_Scenario**: Variable bandwidth links

## How to Run

1. **Setup OMNeT++ Environment**:
   ```bash
   # Set up OMNeT++ environment variables
   source setenv
   ```

2. **Generate Makefiles**:
   ```bash
   make makefiles
   ```

3. **Compile the Project**:
   ```bash
   make
   ```

4. **Run Simulations**:
   ```bash
   # Run OSPF-TE scenario
   ./src/2007106_Network_Project -u Cmdenv -c OSPF_TE_Scenario simulations/omnetpp.ini
   
   # Run RIP scenario
   ./src/2007106_Network_Project -u Cmdenv -c RIP_Scenario simulations/omnetpp.ini
   
   # Run high load scenario
   ./src/2007106_Network_Project -u Cmdenv -c HighLoad_Scenario simulations/omnetpp.ini
   ```

5. **View Results**:
   - Check output files: `ospf_te_results.vec`, `rip_results.vec`
   - Check statistics: `ospf_te_stats.sca`, `rip_stats.sca`
   - Use OMNeT++ result analysis tools

## Expected Results

### OSPF-TE Advantages
- **Faster Convergence**: OSPF-TE typically converges faster than RIP
- **Better Load Balancing**: Routes traffic based on actual link conditions
- **Higher Throughput**: Utilizes high-bandwidth paths more effectively
- **Lower Latency**: Prefers low-delay paths for time-sensitive traffic

### RIP Characteristics
- **Simple Operation**: Easy to configure and understand
- **Lower Overhead**: Less complex than OSPF-TE
- **Slower Convergence**: Takes longer to adapt to network changes
- **Suboptimal Routing**: May not utilize network resources efficiently

## Technical Implementation Details

### OSPF-TE Router
- Implements Traffic Engineering Database (TED)
- Uses Constraint-Based Routing (CBR) algorithm
- Calculates composite metrics: `metric = bwWeight * (1/bandwidth) + delayWeight * delay + loadWeight * load`
- Updates routes dynamically based on network conditions

### RIP Router
- Implements distance vector algorithm
- Uses hop count as routing metric
- Sends periodic updates every 30 seconds
- Maximum hop count of 15

### Performance Monitoring
- Measures throughput, latency, packet loss, and convergence time
- Records statistics in vector and scalar files
- Provides comparative analysis between protocols

## Customization

You can modify the simulation parameters in `simulations/omnetpp.ini`:
- Adjust traffic patterns and rates
- Modify network topology and link characteristics
- Change protocol parameters (timers, weights, etc.)
- Add new scenarios for specific test cases

## Troubleshooting

1. **Compilation Errors**: Ensure all dependencies are properly installed
2. **Runtime Errors**: Check network topology and parameter configurations
3. **Performance Issues**: Adjust simulation time limits and measurement intervals

## Future Enhancements

- Implement more sophisticated TE algorithms
- Add support for MPLS-TE
- Include more traffic types (real-time, bulk transfer)
- Implement advanced performance analysis tools
- Add graphical network visualization
