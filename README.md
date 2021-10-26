# webServerToggle

board A의 코드를 담고 있다.   
board A는 toggle의 값을 가지고 /RelayOn, /RelayOff를 웹콜한다.    
main에 들어있는 소스코드는 toggle을 눌렀을 때만 웹콜을 진행하여 그 값을 시리얼 모니터에 출력을 하게 되는 코드이다.
toggle에 들어 있는 소스코드는 toggle을 누르지 않았을 때도 계속해서 웹콜을 진행한다.   
웹콜에 사용되는 http://192.168.219.104/는 board B의 IP주소이다.


board A를 실행하였을 때 시리얼 모니터에 출력되는 값은 webserver에 나오는 값으로 web에 나오는 값과 같다.    

![image](https://user-images.githubusercontent.com/90883561/138932953-03cfec70-997f-44e4-853f-9077315acd79.png)
![image](https://user-images.githubusercontent.com/90883561/138933027-b9096297-d324-43de-a6c1-3a525888fecb.png)


