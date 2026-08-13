@echo off
echo OSPF-TE vs RIP Network Simulation
echo ==================================

echo.
echo Available scenarios:
echo 1. OSPF_TE_Scenario - OSPF-TE with Traffic Engineering
echo 2. RIP_Scenario - RIP with Hop Count Routing
echo 3. HighLoad_Scenario - High Load Network Conditions
echo 4. LinkFailure_Scenario - Link Failure and Recovery
echo 5. BandwidthVariation_Scenario - Variable Bandwidth Links
echo.

set /p choice="Enter scenario number (1-5): "

if "%choice%"=="1" (
    set scenario=OSPF_TE_Scenario
    echo Running OSPF-TE Scenario...
) else if "%choice%"=="2" (
    set scenario=RIP_Scenario
    echo Running RIP Scenario...
) else if "%choice%"=="3" (
    set scenario=HighLoad_Scenario
    echo Running High Load Scenario...
) else if "%choice%"=="4" (
    set scenario=LinkFailure_Scenario
    echo Running Link Failure Scenario...
) else if "%choice%"=="5" (
    set scenario=BandwidthVariation_Scenario
    echo Running Bandwidth Variation Scenario...
) else (
    echo Invalid choice. Running default OSPF-TE scenario...
    set scenario=OSPF_TE_Scenario
)

echo.
echo Compiling project...
make

if %errorlevel% neq 0 (
    echo Compilation failed. Please check the errors above.
    pause
    exit /b 1
)

echo.
echo Running simulation: %scenario%
echo This may take a few minutes...
echo.

src\2007106_Network_Project.exe -u Cmdenv -c %scenario% simulations\omnetpp.ini

echo.
echo Simulation completed!
echo Check the results in the following files:
echo - *_results.vec (vector data)
echo - *_stats.sca (scalar statistics)
echo.
pause
