# LoraForNepal
Proof-of-concept for Lora 2.4GHz Early Warning System for Nepal
<br>
<br>

## Background
---

Nepal has to deal with the Monsoon every year. This is a period of six months with a lot of rain. Due to this rainfall, it often happens that rivers flood with catastrophic consequences for communities around rivers. With recent past records of disaster during the Monsoon flood, telemetric systems for data monitoring were realized to be essential and found to be successful to help warn the communities. Everyone is well aware of the large number of casualties that the natural disasters like floods have brought in the past, especially in the far Western and Southern regions of Nepal. Even though it is not possible to challenge the force of nature, it is possible to take preventive measures to reduce these numbers. Different efforts have been forwarded in the past from various authorities. The analysis shows a timely dissemination of the river level reading could warn the people early and save thousands of lives and properties. The early warning system for flood detection is being developed for this very purpose. 
<br>
<br>
<br>
![alt text](https://static.dw.com/image/49578114_401.jpg "Monsoon Period")

##### The impact of the monsoon period
<br>
<br>
Robotics Association of Nepal (RAN) has already developed a system to measure water levels and predict floods. The problem they face is that they don't have sufficient technical knowledge to transfer this data to the right community in Nepal. RAN has indicated that they want to develop a LoRa (Long Range) 2.4GHz network to send data from the river to the community. 
<br>
<br>
Diyalo Foundation is committed to the Nepalese population with a focus on helping Nepalese youth to develop technical projects. They have made contact with RAN and from this contact the LoRa early warning flood detection project was created. After conceiving this project, Diyalo Foundation went looking for experts in the field of LoRa. Contact has been made with KPN as they have set up a LoRa network in the frequency band 863 - 870 MHz in the Netherlands. Due to legislation in Nepal, the LoRa network in Nepal will have to function on the 2.4GHz frequency band. Since KPN has no experience with this, the Diyalo Foundation has searched further and eventually ended up with a teacher at Avans Hogeschool. He has accepted this project and has appointed four students of computer science to carry out this project.

This project created by the four students of Avans is a working proof of concept of the early warning flood detection system.  

<br>
<br>

## Working of the project
----

The project is a network made up of LoRa modules each module is a node in the network. The network consists of three types of nodes: a repeater node, a sensor node and a Gateway. Within the network the data is sent from the sensor to the gateway so these are the end-points of the network. Getting the data from the sensor to the gateway is done by repeater nodes, these nodes receive data and when they receive the data they check if the message is a new message and then sends it to every node within its range. This way the messages do not get duplicated and always arrive at their destination. 

![alt text](./images/Network.png "Network") 

The picture above discribes in what way the sensor data gets send through the network. Every normal line is an accepted message that is sent through the network. Every dotted line is a message that the repeaters ignore because the message is already in their backlog.

<br>
<br>

The hardware that is used to make a repeater node is an Arduino Nano Every, the NiceRF LoRa1280F27-TCXO module, a circuit board and an omnidirectional 2dbi antenna. These components work together to create a working programmable LoRa module. This is what the repeater node look like:

![alt text](./images/IMG_20210614_160015_2.jpg "Network") 


Both the gateway and the sensor module do not have a completed module since the right sensor was not available in the Netherlands. De gateway module did not have a completed design because of time constrains.


<br>
<br>


# Disclaimer

EVERYTHING IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.