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

import org.car2x.veins.nodes.RSU;
import org.car2x.veins.nodes.Car;

import org.car2x.veins.nodes.Scenario;
import org.car2x.veins.nodes.TrafficLight;

network Circle extends Scenario
{
    submodules:
        rsu[1]: RSU {
            @display("p=150,140;i=veins/sign/yellowdiamond;is=vs");
        }
		car[2]:Car{
		    @display("p=100,250,row,100");
		}
        tls[0]: TrafficLight {
             @display("p=400,100");
        }
       
        
        
}
