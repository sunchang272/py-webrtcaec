# py-webrtcns
This is a python interface to the WebRTC AEC.

# How to use it

1. Install the webrtcaec module:

```shell
git clone https://github.com/sunchang272/py-webrtcaec.git
cd py-webrtcaec
python setup.py install
```

2. Create a ``AEC`` object:

```python
aec = webrtcaec.aec()
```

2. Give it 2 short segment ("frame") of audio far and near. It only accepts 16-bit mono PCM audio, sampled at 16000Hz. A frame must be 160 samples:

```python
aec.process(far, near)
```
