
import org.car2x.veins.nodes.RSU;
import org.car2x.veins.nodes.Car;
import org.car2x.veins.nodes.Scenario;

network Circle extends Scenario {
    submodules:

        rsu[1]: RSU {
            @display("p=100,200;i=veins/sign/yellowdiamond;is=vs");
        }
        orchestrator: Orchestrator {
            parameters:
                port = 9998;
                         @display("p=300,200;");
            
        }


}