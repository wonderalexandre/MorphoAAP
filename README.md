# MorphoAAP

MorphoAAP is a C++/Python library for extracting **Adaptive Attribute Profiles (AAP)**, a feature extraction technique for pattern recognition, particularly in remote sensing and multichannel image classification.

This library implements the method described in our paper:  

**Wonder A.L. Alves, Wander S. Campos, Charles F. Gobber, Dennis J. Silva, Ronaldo F. Hashimoto**,  
*Multichannel image classification based on adaptive attribute profiles*, Pattern Recognition Letters, 2024,  
DOI: [10.1016/j.patrec.2024.11.015](https://doi.org/10.1016/j.patrec.2024.11.015).  

If you use **MorphoAAP** in your research, please cite our paper.

---

# 🔧 How to Install

MorphoAAP is available on PyPI. You can install it using pip:

```bash
pip install morphoaap
```

---


# 🚀 Example: Extracting AP and AAP

Here is an example of how to extract Attribute Profiles (AP) and Adaptive Attribute Profiles (AAP) using MorphoAAP:

**Step 1: Importing the library**
```python
import numpy as np
import morphoaap as aap
```

**Step 2: Loading an input image**

Represent the input image as a 2D NumPy array. This image will be used as the source for extracting features.

```python
img = np.array([
[122,127,166,201,152, 96, 54, 44, 40, 41, 42, 43, 44, 44, 37],
[133,143,213,246,236,196,137, 85, 55, 43, 44, 45, 35, 40, 42],
[133,168,231,242,246,246,228,172,111, 74, 76, 80, 54, 52, 41],
[147,215,222,199,220,235,244,237,205,172,181,186,106, 57, 47],
[164,235,224,149,168,208,231,244,248,246,246,230,133, 58, 62],
[140,224,237,161,128,149,180,227,245,248,247,243,189,103, 94],
[134,211,240,181,109,105,120,168,223,240,241,246,237,176,110],
[117,188,244,210,111, 74, 86,144,215,230,219,227,232,212,133],
[ 66,159,242,238,149, 75, 78,163,238,212,172,198,219,175,111],
[ 75,144,231,244,171, 81,113,212,222,149,108,115,137,118, 99],
[ 78,139,222,245,185,115,176,229,176, 85, 62, 79, 95, 98,107],
[ 48,102,199,241,220,171,220,208,125, 47, 45, 73, 90, 98,104],
[ 41, 72,171,240,242,233,226,149, 65, 39, 60, 97,104,106,112],
[ 54, 68,140,228,238,236,194,100, 44, 48, 85,100,104,107,122],
[ 54, 54, 94,181,222,214,141, 67, 40, 72, 99,105,106,109,123],
[ 54, 48, 59, 95,145,158, 84, 52, 60, 96,110,115,116,110,113],
[ 49, 45, 44, 48, 71, 89, 49, 47, 71, 95,162,156,119,122,111]
])

num_rows, num_cols = img.shape
img_vector = img.ravel()
```

**Step 3: Creating an AttributeProfile object**

Initialize the AttributeProfile object by providing the flattened image vector and its dimensions.

```python
ap = aap.AttributeProfile(img_vector, num_rows, num_cols)
```

**Step 4: Extracting AP and AAP**

Use the getAP method to extract Attribute Profiles and getAAP for Adaptive Attribute Profiles. The attribute_type parameter defines the type of attribute to be used:
- attribute_type=0 → Area
- attribute_type=1 → Diagonal
- attribute_type=2 → Width of the bounding box
- attribute_type=3 → Height of the bounding box
- attribute_type=4 → Volume

```python
# Parameters
attribute_type = 0  # AREA
thresholds = [10, 50, 100]

# Extract Attribute Profile (AP)
attr_profile = ap.getAP(thresholds, attribute_type)
print("Attribute Profile:")
print(attr_profile)

# Adaptive Attribute Profile
delta_MSER = 10
adap_attr_profile = ap.getAAP(thresholds, attribute_type, delta_MSER)
print("Adaptive Attribute Profile:")
print(adap_attr_profile)
```



**Step 5: Reshaping the output**

Reshape the extracted profiles back into 2D or 3D arrays for visualization.
```python
num_images = adap_attr_profile.shape[1]

# Reshape to 2D arrays
attr_profile_2d = attr_profile.reshape(num_rows, num_cols, num_images)
adap_attr_profile_2d = adap_attr_profile.reshape(num_rows, num_cols, num_images)
```
With these steps, you can extract both AP and AAP from an image and prepare the results for further analysis or visualization.


---


# 📊 Classification of remote sensing data with AAP

- An example of classification of remote sensing data with AAP is available on the *Google Colab* at this 
[link](https://colab.research.google.com/github/wonderalexandre/MorphoAAP/blob/main/example/Classification_of_remote_sensing_data_with_MorphAAP.ipynb)


- The experiments from **Section 5.3: Comparison of AP Variants versus AAP** are available on Google Colab through this [link](https://colab.research.google.com/github/wonderalexandre/MorphoAAP/blob/main/example/Experiments_with_different_APs_methods.ipynb).


