import numpy as np
import morphoaap as aap

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

quant_linhas, quant_colunas = img.shape
img_vetor = img.ravel()

tree = aap.ComponentTree(img_vetor, quant_linhas, quant_colunas, False, 1.5)

def percursoProfundidade(node):
  print("rep:"+str(node.rep), "level:"+str(node.level))
  for child in node.children:
    percursoProfundidade(child)

print("Percurso em profundidade")
percursoProfundidade(tree.root)


attr_area = aap.Attribute.computerArea(tree)
filter = aap.AttributeFilters(tree)
img_filtered = filter.prunningMin(attr_area, 50)
print("Imagem filtrada:")
print( img_filtered )

img_filtered2 = filter.prunningMinByAdaptativeThreshold(attr_area, 50, 1)
print("Imagem filtrada (adap):")
print( img_filtered2 )


ap = aap.AttributeProfile(img_vetor, quant_linhas, quant_colunas)
attr_type = 0 #AREA
thresholds = [10, 50]
attr_profile = ap.getAP(thresholds, attr_type);
print("attribute profile")
print(attr_profile)

adap_attr_profile = ap.getAAP(thresholds, attr_type, 1);
print("adaptive attribute profile")
print(adap_attr_profile)