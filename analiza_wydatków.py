import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from datetime import datetime

df = pd.read_csv('bank.csv', sep=';')
df.columns = [c.replace(' ', '_') for c in df.columns]

df = df[['Data_księgowania', 'Nadawca_/_Odbiorca', 'Kwota_operacji']]
df = df.where(pd.Series(df['Nadawca_/_Odbiorca']).str.contains('ZABKA'))

df = df.dropna()

kwoty = [-float(c.replace(',', '.')) for c in list(df['Kwota_operacji'])]

kwoty = np.cumsum(kwoty)

daty = [datetime.strptime(data, '%d.%m.%Y') for data in df['Data_księgowania']]
daty = daty[::-1]

fig, ax = plt.subplots()
ax.plot(daty, kwoty)
ax.tick_params(axis='both',which="major",labelsize=12)
ax.fill_between(daty, kwoty, facecolor='blue', alpha=0.3)
fig.autofmt_xdate()
plt.show()