import org.car2x.veins.base.modules.IBaseApplLayer;

simple MyRSUApp like IBaseApplLayer 
{
	
    parameters:
        int maxVNFs = default(3);
        int headerLength=80; // length of the application message header (in bits)
        @display("i=block/app");
    gates:
        input lowerLayerIn ; // from network layer
        output lowerLayerOut ; // to network layer
        input lowerControlIn ; // control from network layer
        output lowerControlOut ; // control to network layer
    
}
