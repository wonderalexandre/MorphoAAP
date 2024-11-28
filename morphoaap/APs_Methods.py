# -*- coding: utf-8 -*-


import higra as hg
import numpy as np
import sap
import morphoaap as aap



@hg.argument_helper(hg.CptHierarchy)
@hg.auto_cache
def attribute_diagonal_lenght(tree, leaf_graph):
    if (not hg.CptGridGraph.validate(leaf_graph)) or (len(hg.CptGridGraph.get_shape(leaf_graph)) != 2):
        raise ValueError("Parameter 'leaf_graph' must be a 2D grid graph.")
    coordinates = hg.attribute_vertex_coordinates(leaf_graph)
    coordinates = np.reshape(coordinates, (coordinates.shape[0] * coordinates.shape[1], coordinates.shape[2]))
    coord_max = np.zeros((tree.num_leaves(), 2), dtype=np.float64)
    coord_max[:, 0] = coordinates[:, 0] #linha
    coord_max[:, 1] = coordinates[:, 1] #coluna
    coord_max = hg.accumulate_sequential(tree, coord_max, hg.Accumulators.max)

    coord_min = np.zeros((tree.num_leaves(), 2), dtype=np.float64)
    coord_min[:, 0] = coordinates[:, 0] #linha
    coord_min[:, 1] = coordinates[:, 1] #coluna
    coord_min = hg.accumulate_sequential(tree, coord_min, hg.Accumulators.min)

    height = coord_max[:, 1] - coord_min[:, 1]
    width = coord_max[:, 0] - coord_min[:, 0]
    diagonal_lenght = np.sqrt(height**2 + width**2)

    return diagonal_lenght

hg.attribute_diagonal_lenght = attribute_diagonal_lenght




def compute_AP(image, attr_type, thresholds, adj=4):
    nr, nc, bands = image.shape
    number_of_images_profile = ((len(thresholds) * 2) + 1)
    AP = np.zeros((number_of_images_profile, nr, nc, bands))

    for i in range(bands):
        AP[:,:,:,i] = sap.attribute_profiles(np.ascontiguousarray(image[:,:,i]), {attr_type: thresholds}, adjacency=adj ).data

    final_AP = AP[:,:,:,0]
    for i in range(1, bands):
        final_AP = np.concatenate([ final_AP, AP[:,:,:,i] ],axis=0)

    return final_AP

def compute_MAX(image, attr_type, thresholds, adj=4):
    nr, nc, bands = image.shape
    number_of_images_profile = ((len(thresholds) * 2) + 1)
    AP = np.zeros((number_of_images_profile, nr, nc, bands))

    for i in range(bands):
        AP[:,:,:,i] = sap.attribute_profiles(np.ascontiguousarray(image[:,:,i]), {attr_type: thresholds}, adjacency=adj).data

    final_AP = np.concatenate([AP[len(thresholds):number_of_images_profile,:,:,0]],axis=0)
    for i in range(1, bands):
        final_AP = np.concatenate([final_AP,AP[len(thresholds):number_of_images_profile,:,:,i]],axis=0)

    return final_AP

def compute_MIN(image, attr_type, thresholds, adj=4):
    nr, nc, bands = image.shape
    number_of_images_profile = ((len(thresholds) * 2) + 1)
    AP = np.zeros((number_of_images_profile, nr, nc, bands))

    for i in range(bands):
        AP[:,:,:,i] = sap.attribute_profiles(np.ascontiguousarray(image[:,:,i]), {attr_type: thresholds}, adjacency=adj).data

    final_AP = np.concatenate([AP[0:len(thresholds)+1,:,:,0]],axis=0)
    for i in range(1, bands):
        final_AP = np.concatenate([final_AP,AP[0:len(thresholds)+1,:,:,i]],axis=0)

    return final_AP


def compute_SDAP(image, attr_type, thresholds, adj=4):
    nr, nc, bands = image.shape
    final_SDAP = sap.self_dual_attribute_profiles(np.ascontiguousarray(image[:,:,0]), {attr_type: thresholds}, adjacency=adj)
    for i in range(1, bands):
        SDAP = sap.self_dual_attribute_profiles(np.ascontiguousarray(image[:,:,i]), {attr_type: thresholds}, adjacency=adj)
        final_SDAP = sap.concatenate((final_SDAP,SDAP))

    final_SDAP = sap.vectorize(final_SDAP)

    return final_SDAP

def compute_ALPHA(image, attr_type, thresholds, adj=4):
    nr, nc, bands = image.shape
    ALPHA = sap.profiles.alpha_profiles(np.ascontiguousarray(image[:,:,0]), {attr_type: thresholds}, adjacency=adj)

    ALPHA_profile = sap.concatenate((ALPHA))
    for i in range(1, bands):
        ALPHA = sap.profiles.alpha_profiles(np.ascontiguousarray(image[:,:,i]), {attr_type: thresholds}, adjacency=adj)
        ALPHA_profile = sap.concatenate((ALPHA_profile,ALPHA))

    final_ALPHA = sap.vectorize(ALPHA_profile)

    return final_ALPHA

def compute_OMEGA(image, attr_type, thresholds, adj=4):
    nr, nc, bands = image.shape
    OMEGA = sap.profiles.omega_profiles(np.ascontiguousarray(image[:,:,0]), {attr_type: thresholds}, adjacency=adj)
    OMEGA_profile = sap.concatenate((OMEGA))

    for i in range(1, bands):
        OMEGA = sap.profiles.omega_profiles(np.ascontiguousarray(image[:,:,i]), {attr_type: thresholds}, adjacency=adj)
        OMEGA_profile = sap.concatenate((OMEGA_profile,OMEGA))

    final_OMEGA = sap.vectorize(OMEGA_profile)

    return final_OMEGA

def compute_LFAP(image, attr_type, thresholds, adj=4):
    nr, nc, bands = image.shape
    AP = sap.attribute_profiles(np.ascontiguousarray(image[:,:,0]), {attr_type: thresholds}, adjacency=adj)
    for i in range(1, bands):
        AP = sap.concatenate((AP, sap.attribute_profiles(np.ascontiguousarray(image[:,:,i]), {attr_type: thresholds}, adjacency=adj)))


    LFAP = sap.local_features(AP, local_feature=(np.mean, np.std), patch_size=7) #patch_size changed from 5 to 7 to match with the FP article
    LFAP = sap.vectorize(LFAP)

    return LFAP

def compute_LFSDAP(image, attr_type, thresholds, adj=4):
    nr, nc, bands = image.shape
    SDAP = sap.self_dual_attribute_profiles(np.ascontiguousarray(image[:,:,0]), {attr_type: thresholds}, adjacency=adj)
    for i in range(1, bands):
        SDAP = sap.concatenate((SDAP, sap.self_dual_attribute_profiles(np.ascontiguousarray(image[:,:,i]), {attr_type: thresholds}, adjacency=adj)))

    LFSDAP = sap.local_features(SDAP, local_feature=(np.mean, np.std), patch_size=7) #patch_size changed from 5 to 7 to match with the FP article
    LFSDAP = sap.vectorize(LFSDAP)

    return LFSDAP

def compute_FP(image, attr_type, thresholds, method, adj=4):
    nr, nc, bands = image.shape
    if (method == "FP_ATTR_MEAN"):
        out_feature = {'mean_vertex_weights', attr_type}
    elif (method == "FP_ATTR"):
        out_feature = {attr_type}
    elif (method == "FP_MEAN"):
        out_feature = {'mean_vertex_weights'}

    FP = sap.feature_profiles(np.ascontiguousarray(image[:,:,0]), {attr_type: thresholds}, out_feature=out_feature,adjacency=adj)
    final_FP = sap.concatenate((FP))
    for i in range(1, bands):
        FP = sap.feature_profiles(np.ascontiguousarray(image[:,:,i]), {attr_type: thresholds}, out_feature=out_feature, adjacency=adj)
        final_FP = sap.concatenate((final_FP, FP))

    final_FP = sap.vectorize(final_FP)

    return final_FP


def compute_AAP(image, attr_type, thresholds, deltaMSER, adj=4):
    nr, nc, bands = image.shape
    image_vector = image.reshape((nr * nc, bands))
    number_of_images_profile = ((len(thresholds) * 2) + 1)

    AAP = np.zeros((number_of_images_profile, nr, nc, bands))
    radiusOfAdj = 1 if adj == 4 else 1.5
    if attr_type == 'area':
        attr_type = 0
    elif attr_type == 'diagonal_lenght':
        attr_type = 1
    elif attr_type == 'volume':
        attr_type = 4

    for i in range(bands):
        obj = aap.AttributeProfile(image_vector[:, i], nr, nc, radiusOfAdj)
        if(deltaMSER == 0):
            #tradicional attribute profile
            profile = obj.getAP(thresholds, attr_type)
        else:
            #adaptive attribute profile
            profile = obj.getAAP(thresholds, attr_type, deltaMSER)

        AAP[:,:,:,i] = profile.T.reshape(number_of_images_profile, nr, nc)

    final_AAP = AAP[:,:,:,0]
    for i in range(1, bands):
        final_AAP = np.concatenate([final_AAP, AAP[:,:,:,i]],axis=0)

    return final_AAP
