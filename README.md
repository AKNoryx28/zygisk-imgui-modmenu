# Zygisk-ImGui-ModMenu
Android ImGui with Zygisk, inject imgui at runtime.

## How to use
1. Install [Magisk](https://github.com/topjohnwu/Magisk) v24 or later and enable Zygisk
2. Build module
   - GitHub Actions
      1. Fork this repo
      2. Go to the **Actions** tab in your forked repo
      3. In the left sidebar, click the **Build** workflow.
      4. Above the list of workflow runs, select **Run workflow**
      5. Wait for the action to complete and download the artifact
   - Android Studio
      1. Download the source code
      2. Use Android Studio to run the gradle task `:module:assembleRelease` to compile, the zip package will be generated in the `out` folder
3. Install module in Magisk
4. Start the game

## Credits
Thanks to the following individuals/community whose code
- [Zygisk-Il2CppDumper](https://github.com/Perfare/Zygisk-Il2CppDumper) - [Prefare](https://github.com/Perfare)
- [Dear ImGui](https://github.com/ocornut/imgui) - [omar](https://github.com/ocornut)
- [KittyMemory](https://github.com/MJx0/KittyMemory) - [MJx0](https://github.com/MJx0)
- [Dobby](https://github.com/jmpews/Dobby) - [jmpews](https://github.com/jmpews)
- [xDL](https://github.com/hexhacking/xDL) - [hexhacking](https://github.com/hexhacking)
- [Octowolve](https://github.com/Octowolve/Unity-ImGUI-Android/blob/main/src/utils.cpp)

## Discalimer

This project is for Educational Use only. We do not condone this project being used to gain an advantage against other people. This project was made for fun. If you are using this project for modding/hacking PU_G, C_DM, and any other Tencent games, we ask you to STOP immediately and remove any contents related to it, you will get legal troubles no matter what!

While commecial use/selling is allowed, we still strongly refrain you from buying any source codes on Telegram even if the author can be trusted, there is always a risk getting scammed. We will not be responsible for that.

Use of this service is limited only to non-sensitive and publicly available data. Users must not use, share, or store any kind of sensitive data like health status, provision or payment of healthcare, Personally Identifiable Information (PII) and/or Protected Health Information (PHI), etc. under ANY circumstance.

Administrators for this service reserve the right to moderate all information used, shared, or stored with this service at any time. Any user that cannot abide by this disclaimer and Code of Conduct may be subject to action, up to and including revoking access to services.

The material embodied in this software is provided to you "as-is" and without warranty of any kind, express, implied or otherwise, including without limitation, any warranty of fitness for a particular purpose. In no event shall the Centers for Disease Control and Prevention (CDC) or the United States (U.S.) government be liable to you or anyone else for any direct, special, incidental, indirect or consequential damages of any kind, or any damages whatsoever, including without limitation, loss of profit, loss of use, savings or revenue, or the claims of third parties, whether or not CDC or the U.S. government has been advised of the possibility of such loss, however caused and on any theory of liability, arising out of or in connection with the possession, use or performance of this software.

