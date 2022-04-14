import numpy as np
import json
import matplotlib.pyplot as plt
import os

path_dir = os.path.dirname(__file__)
rel_sig_path = 'finalSignal.json'
rel_sym_path = 'symbol.json'
rel_rrc_path = 'signal.json'


final_signal_path = os.path.join(path_dir, rel_sig_path )
rrc_path = os.path.join(path_dir, rel_rrc_path)
symbol_path = os.path.join(path_dir, rel_sym_path)

data_file = open(final_signal_path) #reads the final signal
data = json.loads(data_file.read())
pulse = np.array(data['real'])+ 1j*np.array(data['imag'])
data_file.close()

data_file = open(rrc_path) #reads the final signal
data = json.loads(data_file.read())
pulse1 = np.array(data['real'])+ 1j*np.array(data['imag'])
data_file.close()

data_file = open(symbol_path) #reads the final signal
data = json.loads(data_file.read())
pulse2 = np.array(data['real'])+ 1j*np.array(data['imag'])
data_file.close()

fig, axs  = plt.subplots(3)

axs[0].plot(pulse)
axs[0].set_title('Final Signal in time domain')
axs[1].plot(pulse1)
axs[1].set_title('Root Raised Cosine impulse in time domain')
axs[2].plot(pulse2)
axs[2].set_title('symbol signal in time domain')
plt.show()
