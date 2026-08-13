network MinimalNetwork
{
    submodules:
        test1: MinimalTest;
        test2: MinimalTest;
    
    connections:
        test1.out --> test2.in;
}
