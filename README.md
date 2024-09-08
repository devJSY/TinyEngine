# TinyEngine

- DirectX11 기반 GameEngine

# Features

- PBR Shader
- MRT, Deferred Rendering
- SSAO
- 물리엔진 라이브러리를 활용한 Physics Simulation
- ImGui 라이브러리를 활용한 각종 Editor
  - Model Editor
  - Material Editor
  - Tilemap Editor
  - Sprite Editor
  - ...    

# Building

1. Download Zip
2. [External 압축 파일](https://drive.google.com/file/d/1-4oStuscaPT3Nftd6fMNFPeWw2Hn0ubE/view?usp=sharing) 다운로드 후 TinyEngine 폴더에 추가 
3. [OutputFile 압축 파일](https://drive.google.com/file/d/1_r-vsvaJEg3sFpUzAtetpwWt6ZwUS3o5/view?usp=drive_link) 다운로드 후 TinyEngine 폴더에 추가
4. Client, Engine, Scripts 프로젝트 → 속성 → 구성 속성 → 디버깅 → 작업 디렉터리 - $(SolutionDir)OutputFile\bin 으로 설정

![image](https://github.com/devJSY/TinyEngine/assets/90514882/f785ce4f-ef69-4535-bcf0-2f48b7cacbb0)

## 폴더 경로 설정

![image](https://github.com/devJSY/TinyEngine/assets/90514882/e88f3422-1968-4bb1-a0e6-d380ca7c420b)

# libraries

- DirectX
- Box2d
- PhysX
- ImGui
- ImGuizmo
- Fbxsdk
- FMOD
