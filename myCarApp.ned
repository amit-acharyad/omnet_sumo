//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

//
// TODO auto-generated module
//
import org.car2x.veins.base.modules.IBaseApplLayer;
simple MyCarApp like IBaseApplLayer
{
    parameters:
        string vnfType=default("OBU");
        int headerLength=80;
        double sendInterval @unit(s);
        @display("i=block/app");
    gates:
 		input lowerLayerIn ; // from network layer
        output lowerLayerOut ; // to network layer
        input lowerControlIn ; // control from network layer
        output lowerControlOut ; // control to network layer
    
}
