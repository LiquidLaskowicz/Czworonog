import os
import sys
import time

import paho.mqtt.client as mqtt

HOST = os.environ.get("BLYNK_HOST", "blynk.cloud")
PORT = 8883  # TLS
KEEPALIVE = 45  # zalecane przez Blynk
GRACE_S = float(os.environ.get("BLYNK_GRACE", "2"))
PINY_SERW = {"V0", "V1", "V2", "V8", "V9"}

polaczono_o = 0.0


def on_connect(client, userdata, flags, reason_code, properties):
    global polaczono_o
    if reason_code.is_failure:
        print(f"Blad polaczenia: {reason_code}", file=sys.stderr)
        return
    polaczono_o = time.monotonic()
    print("Polaczono z Blynkiem (urzadzenie powinno byc online)", file=sys.stderr)
    client.subscribe("downlink/#")


def on_disconnect(client, userdata, disconnect_flags, reason_code, properties):
    print(f"Rozlaczono: {reason_code}", file=sys.stderr)


def on_message(client, userdata, msg):
    tekst = msg.payload.decode(errors="replace").strip()

    if msg.topic == "downlink/redirect":
        # Blynk wskazuje serwer regionalny; uzyj go w BLYNK_HOST
        print(f"Redirect na serwer: {tekst} (ustaw BLYNK_HOST)", file=sys.stderr)
        return

    if not msg.topic.startswith("downlink/ds/"):
        return

    pin = msg.topic.split("/")[-1]

    try:
        wartosc = float(tekst)
    except ValueError:
        print(f"Pomijam {pin}: wartosc '{tekst}' nie jest liczba", file=sys.stderr)
        return

    if pin in PINY_SERW and time.monotonic() - polaczono_o < GRACE_S:
        print(f"Pomijam {pin}={tekst}: ochrona tuz po polaczeniu", file=sys.stderr)
        return

    print(f"{pin} {wartosc:g}", flush=True)


def main():
    token = os.environ.get("BLYNK_TOKEN")
    if not token:
        print("Brak zmiennej srodowiskowej BLYNK_TOKEN", file=sys.stderr)
        return 1

    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
    client.username_pw_set("device", token)
    client.tls_set()
    client.on_connect = on_connect
    client.on_disconnect = on_disconnect
    client.on_message = on_message

    client.connect(HOST, PORT, KEEPALIVE)
    try:
        client.loop_forever()
    except KeyboardInterrupt:
        pass
    finally:
        client.disconnect()
    return 0


if __name__ == "__main__":
    sys.exit(main())