(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or (not v0) (not v1)) (or v0 v1)))
(assert (edge (or v0 (not v2)) (or v3 v4)))
(assert (edge (or v0 (not v1) v2) (or (not v1) v3)))
(assert (edge true (or (not v1) v3 v4)))
(assert (edge (or v0 (not v3) v4) (or v3 (not v0))))
(assert (not (edge (or v4 (not v3)) (or (not v1) v3 (not v0)))))