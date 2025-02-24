# Chat-Program-SharedMemory
IPC 공유메모리를 사용하는 채팅 프로그램

![Image](https://github.com/user-attachments/assets/41b7daa3-3adf-4147-b233-42277c25e7f2)

# 1. 프로젝트 소개 <br/><br/>

- **프로젝트 명**  
  - IPC 공유메모리를 사용하는 채팅 프로그램  <br/><br/>

- **시연 URL**  
  - [youtube 시연](https://youtu.be/cNVF-MrggyE?si=c55l6JyR1YeOXrt8) <br/><br/>

- **프로젝트 동기**  
  - 독립적인 메모리 공간을 갖는 프로세스들을 서로 통신시켜 보기 위해 시작
  - IPC 매커니즘과 공유 메모리 방법으로 채팅 프로그램을 구현하고자 함 <br/><br/>

- **프로젝트 목표**  
  - 다수의 사용자 간 실시간 채팅 시스템을 구현
  - 공유 메모리와 세마포어를 이용한 동기화로 안정적인 메시지 전송과 화면 출력을 제공  <br/><br/>
  
- **프로젝트 설치 및 실행 방법**  
  - 아래 링크에서 ubuntu 설치
    - https://releases.ubuntu.com/
    - 그 외 wsl, virtual box 환경 모두 가능<br/><br/>
  - 아래 명령어로 ncurses library 설치
    - `sudo apt-get install build-essential`
    - `sudo apt-get install libncurses5-dev libncursesw5-dev`
    - `sudo apt-get install gnome-termina`
    - ncurses에 한글을 출력하고 싶을 땐 아래의 라이브러리를 추가로 설치
      - `sudo apt-get install libncursesw5-dev`<br/><br/>
  - shmremove.c, chat.c, chatshm.h를 같은 폴더 위치로 업로드  <br/><br/>
  - `gcc -o shmremove shmremove.c' 로 shmremove 목적 파일 생성 
    - 주의 : shmremove 목적 파일 명이 다르면 안됨 (chat 코드 내부에서 shmremove를 호춣함)  <br/><br/>
  - `gcc -o chat chat.c -lncurses -pthread'로 chat 목적 파일 생성  <br/><br/>
  - `./chat [username]'으로 채팅 프로그램 접속
    - 주의 : 최대 3명까지 동시 접속 가능하도록 설정함 <br/><br/>
  
# 2. 프로젝트 스펙
- **제작 기간**  <br/><br/>
  - 2024.3.26 - 2024.5.17  <br/><br/>
- **팀원 및 역할**  <br/><br/>
  - 개인 프로젝트  <br/><br/>
- **개발 환경**  <br/><br/>
  - 운영체제
    - Ubuntu      (Tested)
    - Linux       (Compatible)
    - UNIX-like   (Compatible, but not tested)  <br/><br/>
  - 사용 언어
    - C 언어  <br/><br/>
- **리포지토리 구조**
  ```
  ├── RainShield
  │   ├── Term_master
  │   │   └── Term_master.ino
  │   └── Term_slave
  │       └── Term_slave.ino
  └── 발표자료&제안서
      ├── 7조_어드벤처디자인_발표자료.pdf
      └── 어벤디 프로젝트 제안서.pdf
  ```
<br/>
  
# 3. 프로젝트 배경지식 <br/><br/>

### 1. IPC와 공유메모리 <br/><br/>

![Image](https://github.com/user-attachments/assets/dba79f51-d2db-47c5-aa44-d79748e15b0c)

- 개념
  - IPC(Inter-Process Communication)는 서로 다른 프로세스 간에 데이터를 전달하고 상호작용할 수 있도록 하는 기술
- 구현
  - IPC 통신을 구현하기 위해 공유메모리(Shared Memory) 방식을 선정
  - 여러 프로세스가 동일한 메모리 영역에 접근하여 데이터를 공유하는 방법
- 장점
  - 데이터를 복사하지 않고 프로세스간에 직접 접근할 수 있기 때문에 데이터 공유가 빠르고 효율적
  - Shared Memory는 하나의 메모리를 공유해서 접근하기 때문에 불필요한 오버헤드가 발생할 확률이 적음
- 단점
  - race condition 발생
  - 동기화를 통해 동시에 접근하지 않도록 제어하여 일관된 결과를 보장해야 함
- 해결방법
  - 세마포어(Semaphore) 동기화 메커니즘을 함께 사용
  - 여러 프로세스가 동시에 공유 자원에 접근하는 것을 막고 오직 한 번에 하나의 프로세스만이 접근할 수 있도록 제어 <br/><br/>

### 2. ncurses 화면
- 서브 윈도우 4개를 2x2 형태로 생성
  - subwin1 : 메시지 출력
  - subwin2 : 로그인된 유저 목록
  - subwin3 : 입력된 메시지 내용
  - subwin4 : 채팅 프로그램 이름 <br/><br/>
  
### 3. 멀티스레드 방식의 채팅 입력과 출력
- 사용자는 메시지를 씀과 동시에 다른 사용자로부터 메시지를 읽을 수도 있음
- 네임드 세마포어를 이용해 유저를 구분
- 읽기 전용 스레드와 화면 출력 전용 스레드를 사용
- 여러 스레드가 동시에 메시지 배열에 접근해 write 동작을 수행한다면 충돌이 발생할 수 있기에
- 메세지를 입력/출력 시 `sem_wait`, `sem_post`를 사용해 동기화시켜서 이 내부 코드의 원자성을 보장함 <br/><br/>

### 4. 시그널
- 채팅 사용자가 ctrl c 입력 시 퇴장이 가능
- ctrl c 입력 시 실행 중인 스레드에 `pthread_cancel`로 취소 요청을 보냄
- 스레드 지연 취소를 이용해 코드가 안전한 상태일 때만 취소되도록 일관성을 유지 <br/><br/>

# 4. 프로젝트 주요 기능 <br/><br/>

### 1. 스크롤 업 

![Image](https://github.com/user-attachments/assets/357f3d2e-7eab-422e-adf8-2a1a8d71279e)

- 메시지 개수가 10개를 초과하면 오래된 메시지를 제거하고 새로운 메시지가 보이도록 배열 간 순서를 조정
- 새로운 메시지가 추가될 때마다 스크롤 업 되는 모습을 구현 <br/><br/>

### 2. 실시간 채팅 (subwin 1) 
- 실시간 채팅 상황이 출력 <br/><br/>

### 3. 접속 중인 유저를 목록에 표시 (subwin 2) 

![Image](https://github.com/user-attachments/assets/5a0cd1f3-8812-495b-bc15-c68726c604c9)

- 유저수가 3명을 넘지 않게 유지하면서 신규 유저가 채팅에 입장하면 목록에 추가
- 공유메모리의에서 유저리스트 목록과 비교해 중복 아이디 생성을 막음
- 이미 유저수가 3명인데 입장하려는 경우 `sem_unlink` 후 종료시켜 입장을 막음
- 입장된 모든 유저는 `/dev/shm` 경로에서 네임드 세마포어로 구분됨 <br/><br/>

### 4. 메시지 입력 창과 출력 창을 구분 (subwin 3) 

![Image](https://github.com/user-attachments/assets/25dd5178-3b25-4f71-bd85-fb1927d77d57)

- 채팅 입력 스레드와 채팅창 출력 스레드간 경쟁 조건이 발생하지 않도록 세마포어로 동기화 <br/><br/>

### 5. ipcs 및 dev/shm 등록 및 해제

**유저 3명 입장 시** <br/><br/>
![Image](https://github.com/user-attachments/assets/fa5d905b-cd6a-4296-a49d-42d4864c4e77)
<br/><br/>
**유저 3명 모두 퇴장 시** <br/><br/>
![Image](https://github.com/user-attachments/assets/bb488bf0-4093-47b9-98b2-f0a3b7819b91)

- 유저가 퇴장 시 `sem_unlink`, `sem_close`로 세마포어를 제거 후 종료
- 채팅방의 인원수가 0명이 되면 내부에서 자동으로 shmremove를 호출해 공유메모리 세그먼트를 제거 <br/><br/>

# 5. 프로젝트 결론 및 리뷰 <br/><br/>

- IPC방식을 통해 프로세스 간 데이터 공유 방법에 대해 익힘
- 실제 채팅 시스템에서 발생할 수 있는 문제들을 해결하는 데 어떻게 적용할 수 있는지 학습
- 동시성 문제를 해결하기 위해 세마포어를 적절하게 활용하는 경험<br/><br/>
