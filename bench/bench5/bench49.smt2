(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or v0 (not v1)) (or v2 (not v3) (not v4))))
(assert (edge (or (not v4) (not v2)) true))
(assert (edge v2 (or (not v1) (not v3))))
(assert (edge true (or v1 v3)))
(assert (edge true (or v3 (not v0))))
(assert (not (edge v2 v3)))