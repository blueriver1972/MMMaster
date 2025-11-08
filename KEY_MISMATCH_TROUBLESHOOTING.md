# 암호화 키 불일치 문제 해결 가이드

## 문제 증상

복호화된 계좌 정보가 깨져 보입니다:
```
AccountID: +??&?"'?nrH (깨진 문자)
LoginID: r??,? (깨진 문자)
```

## 원인

**ConfigGenerator에서 사용한 암호화 키**와 **MMMaster 빌드 시점에 포함된 키**가 일치하지 않습니다.

## 해결 방법

### Step 1: ConfigGenerator로 올바른 키 확인 및 재생성

1. **ConfigGenerator.exe 실행**
2. **동일한 계좌 정보 입력**
3. **암호화 키 입력** (이전에 사용한 키와 동일하게)
   - 예: `MyMasterKey123!`
4. **config.ini와 masterkey.ini 생성 확인**

### Step 2: masterkey.ini 확인

`masterkey.ini` 파일이 프로젝트 루트에 있는지 확인:
```
[MASTER_KEY]
KEY=MyMasterKey123!
```

**중요**: ConfigGenerator에서 입력한 키와 `masterkey.ini`의 키가 일치해야 합니다!

### Step 3: MMMaster 재빌드

1. **Visual Studio에서 솔루션 정리**
   - 빌드 > 솔루션 정리

2. **솔루션 빌드**
   - 빌드 > 솔루션 빌드
   - Pre-Build Event가 `masterkey.ini`를 읽어 `MasterKey.h` 생성
   - 빌드된 실행 파일에 올바른 키 포함

### Step 4: 확인

디버그 로그에서 확인:
```
[DEBUG] ConfigManager::LoadEncryptionKey - Using build-time master key from masterkey.ini (length: 17, sample: MyMasterKe...)
```

키 길이와 샘플이 ConfigGenerator에서 사용한 키와 일치해야 합니다.

## 키 불일치 체크리스트

- [ ] ConfigGenerator에서 사용한 키 확인
- [ ] `masterkey.ini` 파일의 키 확인
- [ ] `MasterKey.h` 파일의 키 확인 (빌드 후)
- [ ] 세 키가 모두 일치하는지 확인
- [ ] MMMaster 재빌드 완료

## 예방 방법

1. **ConfigGenerator 실행 후 즉시 빌드**
   - `masterkey.ini` 생성 후 바로 MMMaster 빌드

2. **키 변경 시 재생성 및 재빌드**
   - 키를 변경하면 ConfigGenerator 재실행
   - MMMaster 재빌드 필수

3. **디버그 로그 확인**
   - 빌드 시점 키 샘플 확인
   - ConfigGenerator 키와 일치하는지 확인

## 디버그 로그 해석

### 정상적인 경우:
```
[DEBUG] ConfigManager::LoadEncryptionKey - Using build-time master key (length: 17, sample: MyMasterKe...)
[DEBUG] ConfigManager::DecryptString - Base64 decoded (input length: 16, decoded length: 11)
[DEBUG] ConfigManager::DecryptString - XOR decryption completed (result length: 11, key length: 17)
[DEBUG] ConfigManager::ReadAccountInfo - Decrypted - AccountIndex: 1, AccountID: 20131719901 (length: 11)
```
→ 복호화 성공, 정상적인 계좌번호 표시

### 키 불일치인 경우:
```
[DEBUG] ConfigManager::LoadEncryptionKey - Using build-time master key (length: 17, sample: TestMaster...)
[DEBUG] ConfigManager::DecryptString - Base64 decoded (input length: 16, decoded length: 11)
[DEBUG] ConfigManager::DecryptString - XOR decryption completed (result length: 11, key length: 17)
[DEBUG] ConfigManager::ReadAccountInfo - Decrypted - AccountIndex: 1, AccountID: +??&?"'?nrH (length: 11)
```
→ 복호화는 되지만 결과가 깨짐 (키 불일치)

## 빠른 해결

1. ConfigGenerator 재실행 (올바른 키로)
2. `masterkey.ini` 확인
3. MMMaster 재빌드
4. 실행 및 디버그 로그 확인

