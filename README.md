# DawnBGC

**Open-Source BGC (Brushless Gimbal Controller) Project**

## How to build in your PC 
<iframe width="560" height="315" src="https://www.youtube.com/embed/LDTULsnp9VM?si=x8lHK7bw_7ZQBUXz" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

## Background

Many years ago, open-source ecosystems for UAV flight control systems had already become mature and highly active. From attitude estimation and state estimation to control and navigation, related technologies have been continuously reused and evolved within the community. However, the **BGC (Brushless Gimbal Controller)**, which is highly coupled with flight controllers and equally critical, still lacks a **complete, engineering-oriented, and sustainably evolving open-source implementation**.

Most existing mainstream BGC solutions are dominated by commercial companies or closed organizations. Their core designs, control architectures, and engineering experiences are rarely fully disclosed. This situation, to some extent, limits the expansion of gimbal technology beyond UAV applications and raises the barrier for researchers and developers to perform secondary development and system integration.

The **DawnBGC project** aims to fill this gap in the open-source ecosystem by providing a learnable, reusable, and extensible reference implementation for gimbal stabilization systems.

---

## Goals

The core goal of DawnBGC is to build an **engineering-oriented open-source BGC system**, and to gradually support its deployment across a wide range of application scenarios, including but not limited to:

- 🚁 UAV and aerial imaging systems  
- 🚗 Advanced driver assistance and vehicle perception platforms  
- 🤖 Industrial robots and mobile platforms  
- 🧍 Wearable stabilization devices  
- 📷 Consumer imaging and stabilization systems  
- 🔭 Telescope stabilization systems  

The project focuses not only on control algorithms, but also covers **low-level drivers, system architecture, upgrade mechanisms, and engineering implementation details**, aiming to form a relatively complete open-source solution.

---

## Overview

DawnBGC currently includes the following major components:

- Core control architecture for gimbal stabilization systems  
- **IAP (In-Application Programming) online firmware upgrade mechanism**  
- Embedded implementation based on **32-bit GD32F405 microcontrollers**  
- Modular and extensible design for easy porting and secondary development  
- Free IDE **GD32 Embedded Builder** [(Download)](https://www.gd32mcu.com/download/agree/box_id/15/document_id/552/path_type/1)
### Supported Features

- ✔ Linear Hall sensor motors  
- ✔ Torque motor drive and control framework  
- ✔ Basic hardware calibration and parameter configuration workflow  
- ✔ IAP online firmware upgrade support  

### Planned Features (Roadmap)

- ⏳ Magnetic encoder support  
- ⏳ Sensorless control solutions  
- ⏳ More comprehensive communication and debugging interfaces  
- ⏳ Hardware Abstraction Layer (HAL) and multi-platform adaptation  

---

## Status & Contribution

Currently, **the DawnBGC project is independently developed and maintained by @MarsDawn**.  
The project is under active development, and both code structure and functionality will continue to evolve based on practical feedback and real-world usage.

Contributions are welcome in the following areas:

- BGC control algorithms and control structure optimization  
- Motor drive and sensor support extensions  
- Hardware platform porting and system abstraction  
- Documentation, examples, and testing workflow improvements  
- Bug fixes and engineering robustness enhancements  

> The value of an open-source community comes from continuous practice and collective contribution.

---

## Hardware

Gimbal stabilization systems are **highly sensitive to hardware design and calibration procedures**.  

Although this is an amateur project, I made attempts to collaborate with numerous hardware manufacturers at an early stage and encountered many thorny issues. So far, only this company has persisted until the project's completion. We sincerely express our gratitude to the hardware suppliers that provided support and reference platforms for the DawnBGC project.

If you would like to quickly verify functionality or conduct demonstration tests, you may obtain validated reference hardware through the following link:

- 👉[**GUC-PRO(USBCamera)**](https://shop471460667.taobao.com/?spm=pc_detail.30350276.shop_block.dshopinfo.357366d4Ofp51t)

---

## ⚠ Safety Notice

**Important Notice:**

This project involves **torque motors, power drivers, and high-speed rotating components**.  
Improper operation during design, debugging, or usage may result in:

- Personal injury  
- Equipment damage  
- Property loss  

**The DawnBGC project and its author assume no responsibility for any direct or indirect damage caused by the use of this project’s code or related designs.**

Please use this project only after fully understanding the system principles, possessing appropriate engineering experience, and implementing necessary safety precautions.

---

## License

Please refer to the `LICENSE` file in this repository for detailed licensing information.
