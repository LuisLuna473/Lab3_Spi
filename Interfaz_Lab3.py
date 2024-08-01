import tkinter as tk
from tkinter import font as tkfont
import serial
import threading
import time

# Configuración del puerto serial
PORT = 'COM3'  # Ajusta esto según el puerto de tu dispositivo
BAUDRATE = 9600

# Crear la instancia del puerto serial
ser = serial.Serial(PORT, BAUDRATE, timeout=1)

# Configuración de la ventana principal
root = tk.Tk()
root.title("Control de Potenciómetros y LEDs")
root.geometry("700x550")  # Tamaño inicial de la ventana
root.resizable(False, False)  # Evita que la ventana se pueda redimensionar
root.configure(bg='#e8f0f2')  # Color de fondo suave

# Fuente personalizada
custom_font = tkfont.Font(family="Times New Roman", size=12, weight="bold")
large_font = tkfont.Font(family="Times New Roman", size=20, weight="bold")  # Aumenta el tamaño de la fuente
button_font = tkfont.Font(family="Times New Roman", size=14, weight="bold")

# Función para crear un cuadro con texto estilo tarjeta
def crear_cuadro(root, texto, color_fondo, color_texto):
    frame = tk.Frame(root, bg=color_fondo, bd=0, relief='flat', padx=10, pady=10)
    
    canvas = tk.Canvas(frame, width=200, height=50, bg=color_fondo, highlightthickness=0, bd=0)
    canvas.create_text(100, 25, text=texto, font=custom_font, fill=color_texto, tag="text")
    canvas.pack()
    
    frame.update_idletasks()
    frame.config(width=canvas.winfo_width() + 20, height=canvas.winfo_height() + 20)
    return frame

# Configuración de los cuadros para los potenciómetros
cuadro_frame = tk.Frame(root, bg='#e8f0f2')
cuadro_frame.pack(pady=20, fill=tk.X, expand=True)

# Añadir un grid a `cuadro_frame` para centrar los cuadros
cuadro_frame.grid_columnconfigure(0, weight=1)
cuadro_frame.grid_rowconfigure(0, weight=1)
cuadro_frame.grid_rowconfigure(1, weight=1)

p1_frame = crear_cuadro(cuadro_frame, "P1: 0", '#ffffff', '#333333')  # Fondo blanco
p2_frame = crear_cuadro(cuadro_frame, "P2: 0", '#ffffff', '#333333')  # Fondo blanco

# Colocar los cuadros en el grid, centrados
p1_frame.grid(row=0, column=0, padx=50, pady=10, sticky="nsew")
p2_frame.grid(row=1, column=0, padx=50, pady=10, sticky="nsew")

# Configuración de los controles para los LEDs
led_value_label_var = tk.StringVar(value="Valor de LEDs: 0")
led_value_label = tk.Label(root, textvariable=led_value_label_var, font=custom_font, bg='#4caf50', fg='#ffffff', padx=10, pady=5, relief='flat')
led_value_label.pack(pady=(10, 0), padx=50, fill=tk.X)  # Ajuste para mover hacia arriba

led_value_entry = tk.Entry(root, font=large_font, width=20, bd=0, relief='flat', highlightthickness=2, highlightcolor='#4caf50')
led_value_entry.pack(pady=20, padx=50, fill=tk.X)  # Aumenta el tamaño del campo

# Funciones de actualización
def actualizar_potenciometros(pot_id):
    """Actualiza el valor mostrado para los potenciómetros"""
    if pot_id == 1:
        ser.write(b'm')  # Envía comando para actualizar el valor de P1
    elif pot_id == 2:
        ser.write(b'n')  # Envía comando para actualizar el valor de P2

def enviar_comando_led():
    """Envía el comando para actualizar los LEDs y actualiza la interfaz gráfica"""
    try:
        valor = int(led_value_entry.get())
        if 0 <= valor <= 255:
            ser.write(f"n{valor}\n".encode())
            led_value_label_var.set(f"Valor de LEDs: {valor}")  # Actualiza la etiqueta con el valor ingresado
            led_value_entry.delete(0, tk.END)  # Limpia el campo de entrada
        else:
            led_value_label_var.set("Valor fuera de rango (0-255)")
            led_value_entry.delete(0, tk.END)  # Limpia el campo de entrada
    except ValueError:
        led_value_label_var.set("Entrada no válida")
        led_value_entry.delete(0, tk.END)  # Limpia el campo de entrada

def leer_datos_serial():
    """Lee datos del puerto serial y actualiza la interfaz"""
    while True:
        if ser.in_waiting > 0:
            try:
                data = ser.readline().decode('utf-8').strip()
                if data.startswith('P1:'):
                    p1_value = data.split(":")[1].strip()
                    root.after(0, lambda: actualizar_texto_canvas(p1_frame, f"P1: {p1_value}"))
                elif data.startswith('P2:'):
                    p2_value = data.split(":")[1].strip()
                    root.after(0, lambda: actualizar_texto_canvas(p2_frame, f"P2: {p2_value}"))
                elif data.startswith('Valor LEDs:'):
                    valor_leds = data.split(":")[1].strip()
                    root.after(0, lambda: led_value_label_var.set(f"Valor de LEDs: {valor_leds}"))
            except UnicodeDecodeError:
                # Ignorar errores de decodificación sin imprimir en consola
                pass
        time.sleep(0.1)  # Pausa para reducir el uso del CPU

def actualizar_texto_canvas(frame, texto):
    """Actualiza el texto en un canvas"""
    canvas = frame.winfo_children()[0]
    canvas.delete("text")
    canvas.create_text(canvas.winfo_width() // 2, canvas.winfo_height() // 2, text=texto, font=custom_font, fill='#333333', tag="text")

# Configuración de los botones cuadrados
def crear_boton(root, text, comando, color_fondo, color_texto, size):
    """Crea un botón cuadrado"""
    boton = tk.Button(root, text=text, command=comando, font=button_font, bg=color_fondo, fg=color_texto, relief='flat', width=size, height=size, padx=0, pady=0)
    return boton

# Crear un marco para los botones
button_frame = tk.Frame(root, bg='#e8f0f2')
button_frame.pack(pady=6, fill=tk.X)

# Crear los botones cuadrados con un tamaño fijo en píxeles
button_pixel_size =7  # Tamaño del botón en términos de cantidad de caracteres (aproximadamente)
send_led_button = crear_boton(button_frame, "Actualizar LEDs", enviar_comando_led, '#ff5722', '#ffffff', button_pixel_size)  # Ajusta el tamaño según sea necesario
update_pots_button = crear_boton(button_frame, "Actualizar Pots", lambda: actualizar_potenciometros(1), '#03a9f4', '#ffffff', button_pixel_size)  # Ajusta el tamaño según sea necesario

# Organizar los botones en el marco usando grid para alinearlos horizontalmente
button_frame.grid_columnconfigure(0, weight=1)
button_frame.grid_columnconfigure(1, weight=1)

send_led_button.grid(row=0, column=0, padx=10, pady=10, sticky="ew")
update_pots_button.grid(row=0, column=1, padx=10, pady=10, sticky="ew")

# Inicia el hilo para leer el puerto serial
def iniciar_thread():
    thread = threading.Thread(target=leer_datos_serial, daemon=True)
    thread.start()

iniciar_thread()

# Ejecuta la interfaz gráfica
root.mainloop()