# Chat-Program-SharedMemory
IPC 공유메모리를 사용하는 채팅 프로그램


통신할 프로세스들이 동일한 기계 상에 있거나 공유 메모리를 사용할 수 있는 환경이어야 한다.
- 채팅프로그램 : asynchronous 하게 만들지 않고 임계영역 빠져나오면 cacnel 시키게 만들기. 여기서 발생하는 문제 테스트 해보기
- 해결방안 : 스레드가 임계영역에 들어가 세마포어를 획득한 상태에서 `pthread_cancel()`에 의해 종료되면, 세마포어가 반환되지 않은 상태로 남을 수 있습니다.
- 이를 방지하기 위해 `pthread_cleanup_push()`와 `pthread_cleanup_pop()`을 사용하여 정리 작업을 수행할 수 있습니다.

# 1. 프로젝트 소개
- **프로젝트 명**  <br/><br/>
  - IPC 공유메모리를 사용하는 채팅 프로그램  <br/><br/>
- **시연 url**  <br/><br/>
  - 시연 url 업로드  <br/><br/>
- **프로젝트 동기**  <br/><br/>
  - IPC 학습
  - 공유 메모리 학습
  - 채팅 프로그램 구현 <br/><br/>
- **프로젝트 목표**  <br/><br/>
  - ㅇㅈ  <br/><br/>
- **프로젝트 설치 및 실행 방법**  <br/><br/>
  - 아래 링크에서 ubuntu 설치
    - https://releases.ubuntu.com/    <br/><br/>
  - 아래 명령어로 ncurses library 설치
    - `sudo apt-get install build-essential`
    - `sudo apt-get install libncurses5-dev libncursesw5-dev`
    - `sudo apt-get install gnome-termina`  <br/><br/>
  - shmremove.c와 chat.c을 같은 폴더 위치로 업로드  <br/><br/>
  - `gcc -o shmremove shmremove.c' 로 shmremove 목적 파일 생성 
    - 주의 : shmremove 목적 파일 명이 다르면 안됨 (chat 코드 내부에서 shmremove를 호춣함)  <br/><br/>
  - `gcc -o chat chat.c -lncurses -pthread'로 chat 목적 파일 생성  <br/><br/>
  - `./chat [username]'으로 채팅 프로그램 접속
    - 주의 : 최대 3명까지 동시 접속 가능하도록 셋팅했음 <br/><br/>
  
# 2. 프로젝트 스펙
- **제작 기간**  <br/><br/>
  - 2024.3.26 - 2024.5.17  <br/><br/>
- **팀원 및 역할**  <br/><br/>
  - 개인 프로젝트  <br/><br/>
- **개발 환경**  <br/><br/>
  - 운영체제
    - Ubuntu (Tested)
    - Linux (Compatible)
    - UNIX-like (Compatible, but not tested)  <br/><br/>
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
  
# 3. 프로젝트 시나리오

# 4. 프로젝트 배경지식

# 5. 프로젝트 주요 기능

# 6. 프로젝트 결론 및 리뷰

# 7. 참고자료
