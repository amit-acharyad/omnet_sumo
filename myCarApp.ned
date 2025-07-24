
import org.car2x.veins.base.modules.IBaseApplLayer;
simple MyCarApp like IBaseApplLayer
{
    parameters:
        string vnfType=default("OBU");
        int headerLength=80;
        double sendInterval @unit(s) = default(3s);  // <-- This is required!
        
        @display("i=block/app");
    gates:
 		input lowerLayerIn ; // from network layer
        output lowerLayerOut ; // to network layer
        input lowerControlIn ; // control from network layer
        output lowerControlOut ; // control to network layer
    
}
