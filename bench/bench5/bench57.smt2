(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge v0 v1))
(assert (edge v2 (or v2 (not v0))))
(assert (edge (or v0 (not v1)) (or (not v1) v3 (not v2))))
(assert (edge (or v2 v3) (or v0 v1 (not v4))))
(assert (edge (or v0 (not v1)) (or v1 (not v2))))
(assert (not (edge true (not v2))))