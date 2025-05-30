import pandas as pd
from sklearn.ensemble import RandomForestClassifier
import json
import os

CSV_FILE = 'sensor_data.csv'
THRESHOLDS_FILE = 'thresholds.json'

# Label unhealthy (1) or healthy (0) based on float thresholds
def label_data(df):
    tempLow, tempHigh = 15.0, 35.0
    humLow, humHigh = 30.0, 90.0  # Updated from 80.0 ? 90.0
    moistLow, moistHigh = 300, 1200

    print("\nMin/Max values:")
    print("Temperature:", df['temperature'].min(), "-", df['temperature'].max())
    print("Humidity:", df['humidity'].min(), "-", df['humidity'].max())
    print("Soil Moisture:", df['soil_moisture'].min(), "-", df['soil_moisture'].max())

    conditions = (
        (df['temperature'] < tempLow) | (df['temperature'] > tempHigh) |
        (df['humidity'] < humLow) | (df['humidity'] > humHigh) |
        (df['soil_moisture'] < moistLow) | (df['soil_moisture'] > moistHigh)
    )
    return conditions.astype(int)

def main():
    print("Reading from:", os.path.abspath(CSV_FILE))

    try:
        df = pd.read_csv(CSV_FILE)
    except Exception as e:
        print("? Error reading CSV:", e)
        return

    expected_cols = {'timestamp', 'temperature', 'humidity', 'soil_moisture'}
    if not expected_cols.issubset(df.columns):
        print("? CSV is missing required columns:", df.columns)
        return

    for col in ['temperature', 'humidity', 'soil_moisture']:
        df[col] = pd.to_numeric(df[col].astype(str).str.strip(), errors='coerce')

    df = df.dropna(subset=['temperature', 'humidity', 'soil_moisture'])

    print("\nData types:\n", df.dtypes)
    print(f"\n? Valid rows after cleaning: {len(df)}")
    if df.empty or len(df) < 10:
        print("? Not enough valid data to train.")
        return

    df['label'] = label_data(df)
    print("\nLabel distribution:\n", df['label'].value_counts())

    X = df[['temperature', 'humidity', 'soil_moisture']]
    y = df['label']
    clf = RandomForestClassifier(n_estimators=100, random_state=42)
    clf.fit(X, y)

    healthy = df[df['label'] == 0]
    if healthy.empty:
        print("? No healthy data found. Can't update thresholds.")
        return

    new_thresholds = {
        'tempLow': round(healthy['temperature'].min().item(), 2),
        'tempHigh': round(healthy['temperature'].max().item(), 2),
        'humLow': round(healthy['humidity'].min().item(), 2),
        'humHigh': round(healthy['humidity'].max().item(), 2),
        'moistLow': int(healthy['soil_moisture'].min().item()),
        'moistHigh': int(healthy['soil_moisture'].max().item())
    }

    with open(THRESHOLDS_FILE, 'w') as f:
        json.dump(new_thresholds, f, indent=4)

    print("\n? Updated thresholds saved to", THRESHOLDS_FILE)
    print(json.dumps(new_thresholds, indent=4))

if __name__ == "__main__":
    main()
