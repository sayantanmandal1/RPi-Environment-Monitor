# Dual-Channel LoRa-Based Smart Environmental Monitoring System

This project presents a **dual-channel smart environmental monitoring system** that combines **LoRa SX1278 long-range wireless communication** with **USB serial data transfer**. It incorporates **edge-based machine learning** for real-time anomaly detection and a **dual-alert mechanism** for prompt user notifications.

## 🚀 Features

- 📡 **Dual Communication Channels**: LoRa wireless (long-range) and USB Serial (high-speed local)
- 🤖 **Machine Learning Integration**: Real-time anomaly detection using Random Forest classification
- 🔄 **Dynamic Threshold Adaptation**: Auto-tunes anomaly detection thresholds every 10 minutes
- ⚡ **Low Power Consumption**: ~45mA during monitoring, ~78mA during alerts
- 💡 **Dual Alert System**: Visual alerts using LEDs and wireless notifications
- 📊 **Real-Time Data Logging**: Periodic environmental data logging with timestamps
- 📈 **Robust Performance Evaluation**: Extensive field-tested range, latency, and accuracy

## 🧠 System Architecture

- **Microcontroller**: Arduino Uno for sensor interfacing
- **Processor**: Raspberry Pi for machine learning and data aggregation
- **LoRa Module**: SX1278 for long-range communication
- **Sensors**: Soil moisture, temperature, and humidity sensors
- **Communication**: LoRa (SPI) and USB Serial (UART)
- **Alerts**: Onboard LEDs and remote transmission

## 🔍 Workflow

1. **Sensor Data Acquisition** from soil moisture, temperature, and humidity sensors
2. **Dual Transmission** via LoRa and USB Serial to the Raspberry Pi
3. **Machine Learning Analysis** using trained Random Forest classifier
4. **Dynamic Threshold Recalibration** every 10 minutes
5. **Alert Triggering** via LEDs and wireless transmission upon anomaly detection
6. **Data Logging** with timestamped environmental metrics

## 📊 Results Summary

- **LoRa Range**: 2.1 km (open), 1.3 km (urban), 1 km (obstructed)
- **Serial Communication**: 115200 baud, 99.9% data success rate
- **ML Accuracy**: 94.2% (Random Forest)
- **Threshold Adaptation**: 67% fewer false positives, 58% better true anomaly detection
- **System Uptime**: 99.2% over 30 days

## 🏆 Performance Comparison

| Feature          | Proposed | LoRaWAN | WiFi-based | Zigbee |
|------------------|----------|---------|------------|--------|
| Range (km)       | 2.1      | 1.8     | 0.1        | 0.3    |
| ML Integration   | ✅ Yes   | ❌ No   | ⚠️ Limited | ❌ No  |
| Dual Channel     | ✅ Yes   | ❌ No   | ❌ No      | ❌ No  |
| Auto Thresholds  | ✅ Yes   | ❌ No   | ❌ No      | ❌ No  |
| Power (mA)       | 45       | 52      | 180        | 85     |
| Accuracy (%)     | 94.2     | 76.5    | 82.3       | 79.1   |

## 🛠️ Technologies Used

- **Arduino** for sensor interfacing and signal transmission
- **Python** on Raspberry Pi for ML and data logging
- **LoRa SX1278** for wireless communication
- **Scikit-learn** for machine learning models
- **Matplotlib / Pandas** for data visualization and analysis

## 🧪 How to Run

1. **Upload Arduino code** to the microcontroller.
2. **Run the Python script** on the Raspberry Pi to start data ingestion and analysis.
3. **Visualize** the results and logs in real-time.
4. **Receive alerts** via LED or remote LoRa signal.

## 📁 File Structure

```
├── arduino/
│   └── lora_transmit.ino
├── raspberry-pi/
│   ├── data_logger.py
│   ├── ml_analyzer.py
│   └── alert_system.py
├── models/
│   └── rf_model.pkl
├── data/
│   └── sensor_logs.csv
└── README.md
```

## 📬 Contact

For questions or collaboration, feel free to reach out:

- Email: yourname@example.com
- GitHub: [yourgithub](https://github.com/yourgithub)
- LinkedIn: [yourprofile](https://linkedin.com/in/yourprofile)

---

© 2025 Dual-Channel Smart Monitoring System. All rights reserved.